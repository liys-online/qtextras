#ifndef QOPENAICHAT_H
#define QOPENAICHAT_H

#include "openai_global.h"
#include "qopenaitypes.h"
#include "qopenairesponse.h"
#include <QObject>
#include <QFuture>
#include <QJsonObject>
#include <QJsonArray>

namespace QOpenAI {

class QOpenAIBaseClient;

// Chat completion create parameters
struct QTEXTRAS_OPENAI_EXPORT ChatCompletionCreateParams {
    // Required
    QString model;
    QList<ChatMessage> messages;
    
    // Optional
    Omit<float> frequencyPenalty;
    Omit<QJsonObject> logitBias;
    Omit<bool> logprobs;
    Omit<int> topLogprobs;
    Omit<int> maxTokens;
    Omit<int> maxCompletionTokens;
    Omit<int> n;
    Omit<QString> modalities; 
    Omit<QJsonObject> prediction;
    Omit<QJsonObject> audio;
    Omit<float> presencePenalty;
    Omit<ResponseFormat> responseFormat;
    Omit<int> seed;
    Omit<QString> serviceTier;
    Omit<QVariant> stop; // Can be QString or QStringList
    Omit<bool> stream;
    Omit<StreamOptions> streamOptions;
    Omit<float> temperature;
    Omit<float> topP;
    Omit<QJsonArray> tools;
    Omit<QVariant> toolChoice;
    Omit<bool> parallelToolCalls;
    Omit<QString> user;
    
    QJsonObject toJson() const;
};

// Completions resource
class QTEXTRAS_OPENAI_EXPORT ChatCompletions : public QObject
{
    Q_OBJECT
public:
    explicit ChatCompletions(QOpenAIBaseClient* client, QObject* parent = nullptr);
    
    // Create a chat completion
    QFuture<ChatCompletionResponse> create(const ChatCompletionCreateParams& params,
                                           const RequestOptions& options = RequestOptions());
    
    // Convenience method with just messages
    QFuture<ChatCompletionResponse> create(const QString& model,
                                          const QList<ChatMessage>& messages,
                                          const RequestOptions& options = RequestOptions());
    
signals:
    void streamChunkReceived(const QJsonObject& chunk);

private:
    QOpenAIBaseClient* m_client;
};

// Chat resource (contains completions)
class QTEXTRAS_OPENAI_EXPORT QChat : public QObject
{
    Q_OBJECT
public:
    explicit QChat(QOpenAIBaseClient* client, QObject* parent = nullptr);
    
    ChatCompletions* completions() { return m_completions; }

private:
    ChatCompletions* m_completions;
};

} // namespace QOpenAI

#endif // QOPENAICHAT_H
