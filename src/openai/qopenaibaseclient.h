#ifndef QOPENAIBASECLIENT_H
#define QOPENAIBASECLIENT_H

#include "openai_global.h"
#include "qopenaitypes.h"
#include "qopenaiexceptions.h"
#include "qopenairesponse.h"
#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFuture>
#include <QPromise>
#include <QTimer>
#include <functional>

namespace QOpenAI {

class QTEXTRAS_OPENAI_EXPORT QOpenAIBaseClient : public QObject
{
    Q_OBJECT
public:
    explicit QOpenAIBaseClient(const OpenAIConfig& config, QObject* parent = nullptr);
    virtual ~QOpenAIBaseClient();

    // Configuration access
    const OpenAIConfig& config() const { return m_config; }
    QString apiKey() const { return m_config.apiKey; }
    void setApiKey(const QString& key) { m_config.apiKey = key; }
    
    // API key refresh
    void refreshApiKey();
    
    // Request building
    QNetworkRequest makeRequest(const QString& path, const RequestOptions& options = RequestOptions()) const;
    
    // HTTP methods
    template<typename ResponseType>
    QFuture<ResponseType> get(const QString& path, const Query& query = Query(), 
                               const RequestOptions& options = RequestOptions());
    
    template<typename ResponseType>
    QFuture<ResponseType> post(const QString& path, const QJsonObject& body = QJsonObject(),
                                const RequestOptions& options = RequestOptions());
    
    template<typename ResponseType>
    QFuture<ResponseType> deleteResource(const QString& path, const RequestOptions& options = RequestOptions());
    
    // Network manager access
    QNetworkAccessManager& networkManager() { return m_manager; }

signals:
    void errorOccurred(const QString& error);

protected:
    OpenAIConfig m_config;
    QNetworkAccessManager m_manager;
    
    // Helper methods
    QString buildUrl(const QString& path, const Query& query = Query()) const;
    void applyHeaders(QNetworkRequest& request, const RequestOptions& options = RequestOptions()) const;
    
    template<typename ResponseType>
    void handleNetworkError(QNetworkReply* reply, QPromise<ResponseType>& promise);
    
    template<typename ResponseType>
    void handleHttpError(int statusCode, const QByteArray& data, QPromise<ResponseType>& promise);
    
    // Retry logic
    template<typename ResponseType>
    QFuture<ResponseType> retryRequest(
        std::function<QNetworkReply*()> requestFunc,
        int retriesLeft,
        const RequestOptions& options = RequestOptions()
    );
};

// Template implementations
template<typename ResponseType>
QFuture<ResponseType> QOpenAIBaseClient::get(const QString& path, const Query& query, 
                                               const RequestOptions& options)
{
    QPromise<ResponseType> promise;
    auto future = promise.future();
    
    refreshApiKey();
    QNetworkRequest req = makeRequest(path, options);
    
    QString url = buildUrl(path, query);
    req.setUrl(QUrl(url));
    
    auto reply = m_manager.get(req);
    
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply, p = std::move(promise)]() mutable {
        if (reply->error() != QNetworkReply::NoError) {
            handleNetworkError(reply, p);
            reply->deleteLater();
            return;
        }
        
        const auto data = reply->readAll();
        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        
        if (statusCode >= 400) {
            handleHttpError(statusCode, data, p);
            reply->deleteLater();
            return;
        }
        
        const auto doc = QJsonDocument::fromJson(data);
        ResponseType response(doc.object());
        
        // Set request ID from header
        QString requestId = reply->rawHeader("x-request-id");
        if (!requestId.isEmpty()) {
            response.setRequestId(requestId);
        }
        
        p.addResult(response);
        p.finish();
        reply->deleteLater();
    });
    
    return future;
}

template<typename ResponseType>
QFuture<ResponseType> QOpenAIBaseClient::post(const QString& path, const QJsonObject& body,
                                                const RequestOptions& options)
{
    QPromise<ResponseType> promise;
    auto future = promise.future();
    
    refreshApiKey();
    QNetworkRequest req = makeRequest(path, options);
    
    auto reply = m_manager.post(req, QJsonDocument(body).toJson());
    
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply, p = std::move(promise)]() mutable {
        if (reply->error() != QNetworkReply::NoError) {
            handleNetworkError(reply, p);
            reply->deleteLater();
            return;
        }
        
        const auto data = reply->readAll();
        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        
        if (statusCode >= 400) {
            handleHttpError(statusCode, data, p);
            reply->deleteLater();
            return;
        }
        
        const auto doc = QJsonDocument::fromJson(data);
        ResponseType response(doc.object());
        
        // Set request ID from header
        QString requestId = reply->rawHeader("x-request-id");
        if (!requestId.isEmpty()) {
            response.setRequestId(requestId);
        }
        
        p.addResult(response);
        p.finish();
        reply->deleteLater();
    });
    
    return future;
}

template<typename ResponseType>
QFuture<ResponseType> QOpenAIBaseClient::deleteResource(const QString& path, const RequestOptions& options)
{
    QPromise<ResponseType> promise;
    auto future = promise.future();
    
    refreshApiKey();
    QNetworkRequest req = makeRequest(path, options);
    
    auto reply = m_manager.deleteResource(req);
    
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply, p = std::move(promise)]() mutable {
        if (reply->error() != QNetworkReply::NoError) {
            handleNetworkError(reply, p);
            reply->deleteLater();
            return;
        }
        
        const auto data = reply->readAll();
        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        
        if (statusCode >= 400) {
            handleHttpError(statusCode, data, p);
            reply->deleteLater();
            return;
        }
        
        const auto doc = QJsonDocument::fromJson(data);
        ResponseType response(doc.object());
        
        QString requestId = reply->rawHeader("x-request-id");
        if (!requestId.isEmpty()) {
            response.setRequestId(requestId);
        }
        
        p.addResult(response);
        p.finish();
        reply->deleteLater();
    });
    
    return future;
}

template<typename ResponseType>
void QOpenAIBaseClient::handleNetworkError(QNetworkReply* reply, QPromise<ResponseType>& promise)
{
    QString errorString = reply->errorString();
    QNetworkReply::NetworkError error = reply->error();
    
    if (error == QNetworkReply::TimeoutError) {
        promise.setException(std::make_exception_ptr(
            APITimeoutError(QString("Request timeout: %1").arg(errorString))
        ));
    } else {
        qDebug() << "Network error occurred:" << error;
        promise.setException(std::make_exception_ptr(
            APIConnectionError(QString("Network error: %1").arg(errorString))
        ));
    }
    
    promise.finish();
    emit errorOccurred(errorString);
}

template<typename ResponseType>
void QOpenAIBaseClient::handleHttpError(int statusCode, const QByteArray& data, QPromise<ResponseType>& promise)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject responseObj = doc.object();
    
    QString errorMessage = responseObj["error"].toObject()["message"].toString();
    if (errorMessage.isEmpty()) {
        errorMessage = QString("HTTP %1 error").arg(statusCode);
    }
    
    std::exception_ptr exceptionPtr;
    
    switch (statusCode) {
        case 400:
            exceptionPtr = std::make_exception_ptr(BadRequestError(errorMessage, responseObj));
            break;
        case 401:
            exceptionPtr = std::make_exception_ptr(AuthenticationError(errorMessage, responseObj));
            break;
        case 403:
            exceptionPtr = std::make_exception_ptr(PermissionDeniedError(errorMessage, responseObj));
            break;
        case 404:
            exceptionPtr = std::make_exception_ptr(NotFoundError(errorMessage, responseObj));
            break;
        case 409:
            exceptionPtr = std::make_exception_ptr(ConflictError(errorMessage, responseObj));
            break;
        case 422:
            exceptionPtr = std::make_exception_ptr(UnprocessableEntityError(errorMessage, responseObj));
            break;
        case 429:
            exceptionPtr = std::make_exception_ptr(RateLimitError(errorMessage, responseObj));
            break;
        default:
            if (statusCode >= 500) {
                exceptionPtr = std::make_exception_ptr(InternalServerError(errorMessage, statusCode, responseObj));
            } else {
                exceptionPtr = std::make_exception_ptr(APIStatusError(errorMessage, statusCode, responseObj));
            }
            break;
    }
    
    promise.setException(exceptionPtr);
    promise.finish();
    emit errorOccurred(errorMessage);
}

} // namespace QOpenAI

#endif // QOPENAIBASECLIENT_H
