#include "qopenairesponse.h"
#include <QJsonArray>

namespace QOpenAI {

// OpenAIResponse implementation
OpenAIResponse::OpenAIResponse(const QJsonObject& raw)
    : m_raw(raw)
{
}

// ChatCompletionResponse implementation
ChatCompletionResponse::ChatCompletionResponse(const QJsonObject& raw)
    : OpenAIResponse(raw)
{
    parse();
}

void ChatCompletionResponse::parse() {
    m_id = m_raw["id"].toString();
    m_object = m_raw["object"].toString();
    m_created = m_raw["created"].toVariant().toLongLong();
    m_model = m_raw["model"].toString();
    m_systemFingerprint = m_raw["system_fingerprint"].toString();
    
    if (m_raw.contains("choices")) {
        QJsonArray choicesArray = m_raw["choices"].toArray();
        for (const auto& choiceValue : choicesArray) {
            m_choices.append(Choice::fromJson(choiceValue.toObject()));
        }
    }
    
    if (m_raw.contains("usage")) {
        m_usage = Usage::fromJson(m_raw["usage"].toObject());
    }
}

QString ChatCompletionResponse::id() const { return m_id; }
QString ChatCompletionResponse::object() const { return m_object; }
qint64 ChatCompletionResponse::created() const { return m_created; }
QString ChatCompletionResponse::model() const { return m_model; }
QList<Choice> ChatCompletionResponse::choices() const { return m_choices; }
Usage ChatCompletionResponse::usage() const { return m_usage; }
QString ChatCompletionResponse::systemFingerprint() const { return m_systemFingerprint; }

QString ChatCompletionResponse::content() const {
    if (!m_choices.isEmpty()) {
        return m_choices.first().message.content;
    }
    return QString();
}

ChatMessage ChatCompletionResponse::message() const {
    if (!m_choices.isEmpty()) {
        return m_choices.first().message;
    }
    return ChatMessage();
}

// CompletionResponse implementation
CompletionResponse::CompletionResponse(const QJsonObject& raw)
    : OpenAIResponse(raw)
{
    parse();
}

void CompletionResponse::parse() {
    m_id = m_raw["id"].toString();
    m_object = m_raw["object"].toString();
    m_created = m_raw["created"].toVariant().toLongLong();
    m_model = m_raw["model"].toString();
    
    if (m_raw.contains("usage")) {
        m_usage = Usage::fromJson(m_raw["usage"].toObject());
    }
}

QJsonArray CompletionResponse::choices() const {
    return m_raw["choices"].toArray();
}

QString CompletionResponse::text() const {
    QJsonArray choices = m_raw["choices"].toArray();
    if (!choices.isEmpty()) {
        return choices.first().toObject()["text"].toString();
    }
    return QString();
}

// EmbeddingResponse implementation
EmbeddingResponse::EmbeddingResponse(const QJsonObject& raw)
    : OpenAIResponse(raw)
{
    parse();
}

void EmbeddingResponse::parse() {
    m_object = m_raw["object"].toString();
    m_model = m_raw["model"].toString();
    
    if (m_raw.contains("usage")) {
        m_usage = Usage::fromJson(m_raw["usage"].toObject());
    }
}

QJsonArray EmbeddingResponse::data() const {
    return m_raw["data"].toArray();
}

QList<QList<float>> EmbeddingResponse::embeddings() const {
    QList<QList<float>> result;
    QJsonArray dataArray = m_raw["data"].toArray();
    
    for (const auto& item : dataArray) {
        QJsonArray embedding = item.toObject()["embedding"].toArray();
        QList<float> vec;
        for (const auto& val : embedding) {
            vec.append(val.toDouble());
        }
        result.append(vec);
    }
    
    return result;
}

// ModelListResponse implementation
ModelListResponse::ModelListResponse(const QJsonObject& raw)
    : OpenAIResponse(raw)
{
    parse();
}

void ModelListResponse::parse() {
    m_object = m_raw["object"].toString();
    
    if (m_raw.contains("data")) {
        QJsonArray dataArray = m_raw["data"].toArray();
        for (const auto& item : dataArray) {
            m_models.append(Model::fromJson(item.toObject()));
        }
    }
}

// FileListResponse implementation
FileListResponse::FileListResponse(const QJsonObject& raw)
    : OpenAIResponse(raw)
{
    parse();
}

void FileListResponse::parse() {
    m_object = m_raw["object"].toString();
    
    if (m_raw.contains("data")) {
        QJsonArray dataArray = m_raw["data"].toArray();
        for (const auto& item : dataArray) {
            m_files.append(FileObject::fromJson(item.toObject()));
        }
    }
}

// FileResponse implementation
FileResponse::FileResponse(const QJsonObject& raw)
    : OpenAIResponse(raw)
{
    parse();
}

void FileResponse::parse() {
    m_file = FileObject::fromJson(m_raw);
}

// DeleteResponse implementation
DeleteResponse::DeleteResponse(const QJsonObject& raw)
    : OpenAIResponse(raw)
{
    parse();
}

void DeleteResponse::parse() {
    m_id = m_raw["id"].toString();
    m_object = m_raw["object"].toString();
    m_deleted = m_raw["deleted"].toBool();
}

// ImageResponse implementation
ImageResponse::ImageResponse(const QJsonObject& raw)
    : OpenAIResponse(raw)
{
    parse();
}

void ImageResponse::parse() {
    m_created = m_raw["created"].toVariant().toLongLong();
}

QJsonArray ImageResponse::data() const {
    return m_raw["data"].toArray();
}

QStringList ImageResponse::urls() const {
    QStringList result;
    QJsonArray dataArray = m_raw["data"].toArray();
    
    for (const auto& item : dataArray) {
        QString url = item.toObject()["url"].toString();
        if (!url.isEmpty()) {
            result.append(url);
        }
    }
    
    return result;
}

QList<QByteArray> ImageResponse::b64Images() const {
    QList<QByteArray> result;
    QJsonArray dataArray = m_raw["data"].toArray();
    
    for (const auto& item : dataArray) {
        QString b64 = item.toObject()["b64_json"].toString();
        if (!b64.isEmpty()) {
            result.append(QByteArray::fromBase64(b64.toUtf8()));
        }
    }
    
    return result;
}

} // namespace QOpenAI
