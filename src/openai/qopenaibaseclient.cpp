#include "qopenaibaseclient.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>

namespace QOpenAI {

QOpenAIBaseClient::QOpenAIBaseClient(const OpenAIConfig& config, QObject* parent)
    : QObject(parent), m_config(config)
{
    // Set default timeout if not configured
    if (m_config.timeout.total == 0) {
        m_config.timeout = Timeout(600.0f); // 10 minutes default
    }
    
    // Configure proxy if specified
    if (m_config.useProxy) {
        m_manager.setProxy(m_config.proxy);
    }
}

QOpenAIBaseClient::~QOpenAIBaseClient()
{
}

void QOpenAIBaseClient::refreshApiKey()
{
    if (m_config.apiKeyProvider) {
        m_config.apiKey = m_config.apiKeyProvider();
    }
}

QString QOpenAIBaseClient::buildUrl(const QString& path, const Query& query) const
{
    QUrl url = m_config.baseUrl.resolved(QUrl(path));
    
    // Add query parameters
    if (!query.isEmpty() || !m_config.defaultQuery.isEmpty()) {
        QUrlQuery urlQuery;
        
        // Add default query params
        for (auto it = m_config.defaultQuery.constBegin(); it != m_config.defaultQuery.constEnd(); ++it) {
            urlQuery.addQueryItem(it.key(), it.value().toString());
        }
        
        // Add request-specific query params
        for (auto it = query.constBegin(); it != query.constEnd(); ++it) {
            urlQuery.addQueryItem(it.key(), it.value().toString());
        }
        
        url.setQuery(urlQuery);
    }
    
    return url.toString();
}
QString requestToString(const QNetworkRequest& request)
{
    QString result;
    QTextStream stream(&result);

    stream << "URL: " << request.url().toString() << '\n';

    for (const auto& header : request.rawHeaderList()) {
        stream << header << ": "
               << request.rawHeader(header) << '\n';
    }

    return result;
}
QNetworkRequest QOpenAIBaseClient::makeRequest(const QString& path, const RequestOptions& options) const
{
    QUrl url = m_config.baseUrl.resolved(QUrl(path));
    QNetworkRequest req(url);
    
    // Set content type
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    // Set authorization header
    if (!m_config.apiKey.isEmpty()) {
        req.setRawHeader("Authorization", QString("Bearer %1").arg(m_config.apiKey).toUtf8());
    }
    
    // Set organization header
    if (!m_config.organization.isEmpty()) {
        req.setRawHeader("OpenAI-Organization", m_config.organization.toUtf8());
    }
    
    // Set project header
    if (!m_config.project.isEmpty()) {
        req.setRawHeader("OpenAI-Project", m_config.project.toUtf8());
    }
    
    // Set user agent
    // req.setRawHeader("User-Agent", "QOpenAI/1.0.0");
    
    // Apply default headers
    for (auto it = m_config.defaultHeaders.constBegin(); it != m_config.defaultHeaders.constEnd(); ++it) {
        req.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }
    
    // Apply extra headers from options
    if (options.extraHeaders.hasValue()) {
        const Headers& extraHeaders = options.extraHeaders.getValue();
        for (auto it = extraHeaders.constBegin(); it != extraHeaders.constEnd(); ++it) {
            req.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
        }
    }
    
    // Set timeout
    Timeout timeout = m_config.timeout;
    if (options.timeout.hasValue()) {
        timeout = options.timeout.getValue();
    }
    req.setTransferTimeout(static_cast<int>(timeout.total * 1000));
    qDebug() << requestToString(req);
    return req;
}

void QOpenAIBaseClient::applyHeaders(QNetworkRequest& request, const RequestOptions& options) const
{
    if (options.extraHeaders.hasValue()) {
        const Headers& extraHeaders = options.extraHeaders.getValue();
        for (auto it = extraHeaders.constBegin(); it != extraHeaders.constEnd(); ++it) {
            request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
        }
    }
}

// Template method implementations are in the header file

} // namespace QOpenAI
