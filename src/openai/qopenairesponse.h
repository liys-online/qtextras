#ifndef QOPENAIRESPONSE_H
#define QOPENAIRESPONSE_H

#include "openai_global.h"
#include "qopenaitypes.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QList>

namespace QOpenAI {

// Base response class
class QTEXTRAS_OPENAI_EXPORT OpenAIResponse {
public:
    explicit OpenAIResponse(const QJsonObject& raw = QJsonObject());
    virtual ~OpenAIResponse() = default;

    QJsonObject raw() const { return m_raw; }
    QString requestId() const { return m_requestId; }
    void setRequestId(const QString& id) { m_requestId = id; }

protected:
    QJsonObject m_raw;
    QString m_requestId;
};

// Chat completion response
class QTEXTRAS_OPENAI_EXPORT ChatCompletionResponse : public OpenAIResponse {
public:
    explicit ChatCompletionResponse(const QJsonObject& raw = QJsonObject());

    QString id() const;
    QString object() const;
    qint64 created() const;
    QString model() const;
    QList<Choice> choices() const;
    Usage usage() const;
    QString systemFingerprint() const;
    
    // Convenience methods
    QString content() const; // Get first choice content
    ChatMessage message() const; // Get first choice message

private:
    void parse();
    
    QString m_id;
    QString m_object;
    qint64 m_created;
    QString m_model;
    QList<Choice> m_choices;
    Usage m_usage;
    QString m_systemFingerprint;
};

// Completion response (legacy)
class QTEXTRAS_OPENAI_EXPORT CompletionResponse : public OpenAIResponse {
public:
    explicit CompletionResponse(const QJsonObject& raw = QJsonObject());

    QString id() const { return m_id; }
    QString object() const { return m_object; }
    qint64 created() const { return m_created; }
    QString model() const { return m_model; }
    QJsonArray choices() const;
    Usage usage() const { return m_usage; }
    
    QString text() const; // Get first choice text

private:
    void parse();
    
    QString m_id;
    QString m_object;
    qint64 m_created;
    QString m_model;
    Usage m_usage;
};

// Embedding response
class QTEXTRAS_OPENAI_EXPORT EmbeddingResponse : public OpenAIResponse {
public:
    explicit EmbeddingResponse(const QJsonObject& raw = QJsonObject());

    QString object() const { return m_object; }
    QString model() const { return m_model; }
    QJsonArray data() const;
    Usage usage() const { return m_usage; }
    
    QList<QList<float>> embeddings() const;

private:
    void parse();
    
    QString m_object;
    QString m_model;
    Usage m_usage;
};

// Model list response
class QTEXTRAS_OPENAI_EXPORT ModelListResponse : public OpenAIResponse {
public:
    explicit ModelListResponse(const QJsonObject& raw = QJsonObject());

    QString object() const { return m_object; }
    QList<Model> models() const { return m_models; }

private:
    void parse();
    
    QString m_object;
    QList<Model> m_models;
};

// File list response
class QTEXTRAS_OPENAI_EXPORT FileListResponse : public OpenAIResponse {
public:
    explicit FileListResponse(const QJsonObject& raw = QJsonObject());

    QString object() const { return m_object; }
    QList<FileObject> files() const { return m_files; }

private:
    void parse();
    
    QString m_object;
    QList<FileObject> m_files;
};

// File response
class QTEXTRAS_OPENAI_EXPORT FileResponse : public OpenAIResponse {
public:
    explicit FileResponse(const QJsonObject& raw = QJsonObject());

    FileObject file() const { return m_file; }

private:
    void parse();
    
    FileObject m_file;
};

// Delete response
class QTEXTRAS_OPENAI_EXPORT DeleteResponse : public OpenAIResponse {
public:
    explicit DeleteResponse(const QJsonObject& raw = QJsonObject());

    QString id() const { return m_id; }
    QString object() const { return m_object; }
    bool deleted() const { return m_deleted; }

private:
    void parse();
    
    QString m_id;
    QString m_object;
    bool m_deleted = false;
};

// Image response
class QTEXTRAS_OPENAI_EXPORT ImageResponse : public OpenAIResponse {
public:
    explicit ImageResponse(const QJsonObject& raw = QJsonObject());

    qint64 created() const { return m_created; }
    QJsonArray data() const;
    
    QStringList urls() const;
    QList<QByteArray> b64Images() const;

private:
    void parse();
    
    qint64 m_created = 0;
};

} // namespace QOpenAI

#endif // QOPENAIRESPONSE_H
