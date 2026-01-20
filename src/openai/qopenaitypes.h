#ifndef QOPENAITYPES_H
#define QOPENAITYPES_H

#include "openai_global.h"
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QMap>
#include <QNetworkProxy>
#include <functional>

namespace QOpenAI {

// Forward declarations
class OpenAIResponse;
class StreamEvent;

// Type aliases
using Headers = QMap<QString, QString>;
using Query = QMap<QString, QVariant>;

// NotGiven sentinel for optional parameters
struct NotGiven {
    static NotGiven instance() { return NotGiven{}; }
    bool operator==(const NotGiven&) const { return true; }
};

// Omit type for parameters that can be omitted
template<typename T>
struct Omit {
    T value;
    bool isOmitted = true;
    
    Omit() : isOmitted(true) {}
    Omit(const T& v) : value(v), isOmitted(false) {}
    Omit(const NotGiven&) : isOmitted(true) {}
    
    bool hasValue() const { return !isOmitted; }
    T getValue() const { return value; }
};

// Timeout configuration
struct QTEXTRAS_OPENAI_EXPORT Timeout {
    float total = 600.0f;      // 10 minutes default
    float connect = 60.0f;
    float read = 60.0f;
    float write = 60.0f;
    
    Timeout() = default;
    explicit Timeout(float t) : total(t) {}
    Timeout(float c, float r, float w) : connect(c), read(r), write(w) {}
};

// Request options that can be passed per-request
struct QTEXTRAS_OPENAI_EXPORT RequestOptions {
    Omit<Headers> extraHeaders;
    Omit<Query> extraQuery;
    Omit<QJsonObject> extraBody;
    Omit<Timeout> timeout;
    Omit<int> maxRetries;
};

// OpenAI Configuration
struct QTEXTRAS_OPENAI_EXPORT OpenAIConfig {
    QString apiKey = "sk-6b4454684ad94765be82c34305e0b14f";
    // QString apiKey = "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";

    std::function<QString()> apiKeyProvider;

    QString organization;
    QString project;
    QString webhookSecret;

    QUrl baseUrl = QUrl("https://api.deepseek.com");
    // QUrl baseUrl = QUrl("https://api.openai.com/v1");
    QUrl websocketBaseUrl;

    Timeout timeout;
    int maxRetries = 2;

    Headers defaultHeaders;
    Query defaultQuery;
    
    bool strictResponseValidation = false;
    
    // Proxy configuration
    QNetworkProxy proxy;
    bool useProxy = false;
};

// Chat message role
enum class MessageRole {
    System,
    User,
    Assistant,
    Tool,
    Function,
    Developer
};

inline QString messageRoleToString(MessageRole role) {
    switch (role) {
        case MessageRole::System: return "system";
        case MessageRole::User: return "user";
        case MessageRole::Assistant: return "assistant";
        case MessageRole::Tool: return "tool";
        case MessageRole::Function: return "function";
        case MessageRole::Developer: return "developer";
    }
    return "user";
}

// Message content types
struct QTEXTRAS_OPENAI_EXPORT TextContent {
    QString type = "text";
    QString text;
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["text"] = text;
        return obj;
    }
};

struct QTEXTRAS_OPENAI_EXPORT ImageUrl {
    QString url;
    QString detail; // "auto", "low", "high"
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["url"] = url;
        if (!detail.isEmpty()) {
            obj["detail"] = detail;
        }
        return obj;
    }
};

struct QTEXTRAS_OPENAI_EXPORT ImageContent {
    QString type = "image_url";
    ImageUrl imageUrl;
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["image_url"] = imageUrl.toJson();
        return obj;
    }
};

// Chat message
struct QTEXTRAS_OPENAI_EXPORT ChatMessage {
    MessageRole role = MessageRole::User;
    QString content;
    QJsonArray contentArray; // For multi-modal content
    QString name;
    QString toolCallId;
    QJsonArray toolCalls;
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["role"] = messageRoleToString(role);
        
        if (!contentArray.isEmpty()) {
            obj["content"] = contentArray;
        } else if (!content.isEmpty()) {
            obj["content"] = content;
        }
        
        if (!name.isEmpty()) {
            obj["name"] = name;
        }
        if (!toolCallId.isEmpty()) {
            obj["tool_call_id"] = toolCallId;
        }
        if (!toolCalls.isEmpty()) {
            obj["tool_calls"] = toolCalls;
        }
        
        return obj;
    }
    
    static ChatMessage fromJson(const QJsonObject& obj) {
        ChatMessage msg;
        QString roleStr = obj["role"].toString();
        if (roleStr == "system") msg.role = MessageRole::System;
        else if (roleStr == "assistant") msg.role = MessageRole::Assistant;
        else if (roleStr == "tool") msg.role = MessageRole::Tool;
        else if (roleStr == "function") msg.role = MessageRole::Function;
        else if (roleStr == "developer") msg.role = MessageRole::Developer;
        else msg.role = MessageRole::User;
        
        if (obj["content"].isString()) {
            msg.content = obj["content"].toString();
        } else if (obj["content"].isArray()) {
            msg.contentArray = obj["content"].toArray();
        }
        
        msg.name = obj["name"].toString();
        msg.toolCallId = obj["tool_call_id"].toString();
        if (obj.contains("tool_calls")) {
            msg.toolCalls = obj["tool_calls"].toArray();
        }
        
        return msg;
    }
};

// Function call
struct QTEXTRAS_OPENAI_EXPORT FunctionCall {
    QString name;
    QString arguments;
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["name"] = name;
        obj["arguments"] = arguments;
        return obj;
    }
    
    static FunctionCall fromJson(const QJsonObject& obj) {
        FunctionCall fc;
        fc.name = obj["name"].toString();
        fc.arguments = obj["arguments"].toString();
        return fc;
    }
};

// Tool choice
enum class ToolChoiceType {
    None,
    Auto,
    Required,
    Function
};

// Response format
enum class ResponseFormatType {
    Text,
    JsonObject,
    JsonSchema
};

struct QTEXTRAS_OPENAI_EXPORT ResponseFormat {
    ResponseFormatType type = ResponseFormatType::Text;
    QJsonObject jsonSchema;
    
    QJsonObject toJson() const {
        QJsonObject obj;
        switch (type) {
            case ResponseFormatType::Text:
                obj["type"] = "text";
                break;
            case ResponseFormatType::JsonObject:
                obj["type"] = "json_object";
                break;
            case ResponseFormatType::JsonSchema:
                obj["type"] = "json_schema";
                if (!jsonSchema.isEmpty()) {
                    obj["json_schema"] = jsonSchema;
                }
                break;
        }
        return obj;
    }
};

// Stream options
struct QTEXTRAS_OPENAI_EXPORT StreamOptions {
    bool includeUsage = false;
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["include_usage"] = includeUsage;
        return obj;
    }
};

// Choice
struct QTEXTRAS_OPENAI_EXPORT Choice {
    int index = 0;
    ChatMessage message;
    QString finishReason;
    QJsonObject logprobs;
    
    static Choice fromJson(const QJsonObject& obj) {
        Choice choice;
        choice.index = obj["index"].toInt();
        if (obj.contains("message")) {
            choice.message = ChatMessage::fromJson(obj["message"].toObject());
        }
        choice.finishReason = obj["finish_reason"].toString();
        if (obj.contains("logprobs")) {
            choice.logprobs = obj["logprobs"].toObject();
        }
        return choice;
    }
};

// Usage information
struct QTEXTRAS_OPENAI_EXPORT Usage {
    int promptTokens = 0;
    int completionTokens = 0;
    int totalTokens = 0;
    
    static Usage fromJson(const QJsonObject& obj) {
        Usage usage;
        usage.promptTokens = obj["prompt_tokens"].toInt();
        usage.completionTokens = obj["completion_tokens"].toInt();
        usage.totalTokens = obj["total_tokens"].toInt();
        return usage;
    }
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["prompt_tokens"] = promptTokens;
        obj["completion_tokens"] = completionTokens;
        obj["total_tokens"] = totalTokens;
        return obj;
    }
};

// Model information
struct QTEXTRAS_OPENAI_EXPORT Model {
    QString id;
    QString object;
    qint64 created = 0;
    QString ownedBy;
    
    static Model fromJson(const QJsonObject& obj) {
        Model model;
        model.id = obj["id"].toString();
        model.object = obj["object"].toString();
        model.created = obj["created"].toVariant().toLongLong();
        model.ownedBy = obj["owned_by"].toString();
        return model;
    }
};

// File information
struct QTEXTRAS_OPENAI_EXPORT FileObject {
    QString id;
    QString object;
    qint64 bytes = 0;
    qint64 createdAt = 0;
    QString filename;
    QString purpose;
    QString status;
    
    static FileObject fromJson(const QJsonObject& obj) {
        FileObject file;
        file.id = obj["id"].toString();
        file.object = obj["object"].toString();
        file.bytes = obj["bytes"].toVariant().toLongLong();
        file.createdAt = obj["created_at"].toVariant().toLongLong();
        file.filename = obj["filename"].toString();
        file.purpose = obj["purpose"].toString();
        file.status = obj["status"].toString();
        return file;
    }
};

} // namespace QOpenAI

#endif // QOPENAITYPES_H
