#ifndef QOPENAIEXCEPTIONS_H
#define QOPENAIEXCEPTIONS_H

#include "openai_global.h"
#include <QException>
#include <QString>
#include <QJsonObject>
#include <stdexcept>

namespace QOpenAI {

// Base exception class for all OpenAI errors
class QTEXTRAS_OPENAI_EXPORT OpenAIError : public std::runtime_error {
public:
    explicit OpenAIError(const QString& message)
        : std::runtime_error(message.toStdString()), m_message(message) {}
    
    QString message() const { return m_message; }
    
protected:
    QString m_message;
};

// API connection errors
class QTEXTRAS_OPENAI_EXPORT APIConnectionError : public OpenAIError {
public:
    explicit APIConnectionError(const QString& message)
        : OpenAIError(message) {}
};

// API timeout errors
class QTEXTRAS_OPENAI_EXPORT APITimeoutError : public OpenAIError {
public:
    explicit APITimeoutError(const QString& message)
        : OpenAIError(message) {}
};

// API status errors (4xx, 5xx)
class QTEXTRAS_OPENAI_EXPORT APIStatusError : public OpenAIError {
public:
    APIStatusError(const QString& message, int statusCode, const QJsonObject& response = QJsonObject())
        : OpenAIError(message), m_statusCode(statusCode), m_response(response) {}
    
    int statusCode() const { return m_statusCode; }
    QJsonObject response() const { return m_response; }
    
protected:
    int m_statusCode;
    QJsonObject m_response;
};

// 400 Bad Request
class QTEXTRAS_OPENAI_EXPORT BadRequestError : public APIStatusError {
public:
    explicit BadRequestError(const QString& message, const QJsonObject& response = QJsonObject())
        : APIStatusError(message, 400, response) {}
};

// 401 Unauthorized
class QTEXTRAS_OPENAI_EXPORT AuthenticationError : public APIStatusError {
public:
    explicit AuthenticationError(const QString& message, const QJsonObject& response = QJsonObject())
        : APIStatusError(message, 401, response) {}
};

// 403 Forbidden
class QTEXTRAS_OPENAI_EXPORT PermissionDeniedError : public APIStatusError {
public:
    explicit PermissionDeniedError(const QString& message, const QJsonObject& response = QJsonObject())
        : APIStatusError(message, 403, response) {}
};

// 404 Not Found
class QTEXTRAS_OPENAI_EXPORT NotFoundError : public APIStatusError {
public:
    explicit NotFoundError(const QString& message, const QJsonObject& response = QJsonObject())
        : APIStatusError(message, 404, response) {}
};

// 409 Conflict
class QTEXTRAS_OPENAI_EXPORT ConflictError : public APIStatusError {
public:
    explicit ConflictError(const QString& message, const QJsonObject& response = QJsonObject())
        : APIStatusError(message, 409, response) {}
};

// 422 Unprocessable Entity
class QTEXTRAS_OPENAI_EXPORT UnprocessableEntityError : public APIStatusError {
public:
    explicit UnprocessableEntityError(const QString& message, const QJsonObject& response = QJsonObject())
        : APIStatusError(message, 422, response) {}
};

// 429 Rate Limit
class QTEXTRAS_OPENAI_EXPORT RateLimitError : public APIStatusError {
public:
    explicit RateLimitError(const QString& message, const QJsonObject& response = QJsonObject())
        : APIStatusError(message, 429, response) {}
};

// 500+ Server Error
class QTEXTRAS_OPENAI_EXPORT InternalServerError : public APIStatusError {
public:
    explicit InternalServerError(const QString& message, int statusCode, const QJsonObject& response = QJsonObject())
        : APIStatusError(message, statusCode, response) {}
};

// Response validation error
class QTEXTRAS_OPENAI_EXPORT APIResponseValidationError : public OpenAIError {
public:
    explicit APIResponseValidationError(const QString& message)
        : OpenAIError(message) {}
};

// Webhook signature error
class QTEXTRAS_OPENAI_EXPORT InvalidWebhookSignatureError : public OpenAIError {
public:
    explicit InvalidWebhookSignatureError(const QString& message)
        : OpenAIError(message) {}
};

// Content filter finish reason error
class QTEXTRAS_OPENAI_EXPORT ContentFilterFinishReasonError : public OpenAIError {
public:
    explicit ContentFilterFinishReasonError(const QString& message)
        : OpenAIError(message) {}
};

// Length finish reason error
class QTEXTRAS_OPENAI_EXPORT LengthFinishReasonError : public OpenAIError {
public:
    explicit LengthFinishReasonError(const QString& message)
        : OpenAIError(message) {}
};

} // namespace QOpenAI

#endif // QOPENAIEXCEPTIONS_H
