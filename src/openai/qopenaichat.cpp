#include "qopenaichat.h"
#include "qopenaibaseclient.h"
#include <QJsonDocument>

namespace QOpenAI {

// ChatCompletionCreateParams implementation
QJsonObject ChatCompletionCreateParams::toJson() const
{
    QJsonObject obj;
    
    // Required fields
    obj["model"] = model;
    
    QJsonArray messagesArray;
    for (const auto& msg : messages) {
        messagesArray.append(msg.toJson());
    }
    obj["messages"] = messagesArray;
    
    // Optional fields
    if (frequencyPenalty.hasValue()) {
        obj["frequency_penalty"] = frequencyPenalty.getValue();
    }
    if (logitBias.hasValue()) {
        obj["logit_bias"] = logitBias.getValue();
    }
    if (logprobs.hasValue()) {
        obj["logprobs"] = logprobs.getValue();
    }
    if (topLogprobs.hasValue()) {
        obj["top_logprobs"] = topLogprobs.getValue();
    }
    if (maxTokens.hasValue()) {
        obj["max_tokens"] = maxTokens.getValue();
    }
    if (maxCompletionTokens.hasValue()) {
        obj["max_completion_tokens"] = maxCompletionTokens.getValue();
    }
    if (n.hasValue()) {
        obj["n"] = n.getValue();
    }
    if (modalities.hasValue()) {
        obj["modalities"] = modalities.getValue();
    }
    if (prediction.hasValue()) {
        obj["prediction"] = prediction.getValue();
    }
    if (audio.hasValue()) {
        obj["audio"] = audio.getValue();
    }
    if (presencePenalty.hasValue()) {
        obj["presence_penalty"] = presencePenalty.getValue();
    }
    if (responseFormat.hasValue()) {
        obj["response_format"] = responseFormat.getValue().toJson();
    }
    if (seed.hasValue()) {
        obj["seed"] = seed.getValue();
    }
    if (serviceTier.hasValue()) {
        obj["service_tier"] = serviceTier.getValue();
    }
    if (stop.hasValue()) {
        QVariant stopValue = stop.getValue();
        if (stopValue.type() == QVariant::String) {
            obj["stop"] = stopValue.toString();
        } else if (stopValue.type() == QVariant::StringList) {
            QJsonArray stopArray;
            for (const QString& s : stopValue.toStringList()) {
                stopArray.append(s);
            }
            obj["stop"] = stopArray;
        }
    }
    if (stream.hasValue()) {
        obj["stream"] = stream.getValue();
    }
    if (streamOptions.hasValue()) {
        obj["stream_options"] = streamOptions.getValue().toJson();
    }
    if (temperature.hasValue()) {
        obj["temperature"] = temperature.getValue();
    }
    if (topP.hasValue()) {
        obj["top_p"] = topP.getValue();
    }
    if (tools.hasValue()) {
        obj["tools"] = tools.getValue();
    }
    if (toolChoice.hasValue()) {
        QVariant tc = toolChoice.getValue();
        if (tc.type() == QVariant::String) {
            obj["tool_choice"] = tc.toString();
        } else if (tc.canConvert<QJsonObject>()) {
            obj["tool_choice"] = tc.toJsonObject();
        }
    }
    if (parallelToolCalls.hasValue()) {
        obj["parallel_tool_calls"] = parallelToolCalls.getValue();
    }
    if (user.hasValue()) {
        obj["user"] = user.getValue();
    }
    
    return obj;
}

// ChatCompletions implementation
ChatCompletions::ChatCompletions(QOpenAIBaseClient* client, QObject* parent)
    : QObject(parent), m_client(client)
{
}

QFuture<ChatCompletionResponse> ChatCompletions::create(const ChatCompletionCreateParams& params,
                                                         const RequestOptions& options)
{
    QJsonObject body = params.toJson();
    return m_client->post<ChatCompletionResponse>("v1/chat/completions", body, options);
}

QFuture<ChatCompletionResponse> ChatCompletions::create(const QString& model,
                                                        const QList<ChatMessage>& messages,
                                                        const RequestOptions& options)
{
    ChatCompletionCreateParams params;
    params.model = model;
    params.messages = messages;
    return create(params, options);
}

// QChat implementation
QChat::QChat(QOpenAIBaseClient* client, QObject* parent)
    : QObject(parent)
{
    m_completions = new ChatCompletions(client, this);
}

} // namespace QOpenAI
