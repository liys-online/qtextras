#include "qopenairesources.h"
#include "qopenaibaseclient.h"
#include <QJsonDocument>
#include <QHttpMultiPart>
#include <QHttpPart>

namespace QOpenAI {

// ============================================================================
// Completions Implementation
// ============================================================================

QJsonObject CompletionCreateParams::toJson() const
{
    QJsonObject obj;
    obj["model"] = model;
    
    // Handle prompt (can be string, array of strings, or array of ints)
    if (prompt.type() == QVariant::String) {
        obj["prompt"] = prompt.toString();
    } else if (prompt.type() == QVariant::StringList) {
        QJsonArray arr;
        for (const QString& s : prompt.toStringList()) {
            arr.append(s);
        }
        obj["prompt"] = arr;
    } else if (prompt.canConvert<QJsonArray>()) {
        obj["prompt"] = prompt.toJsonArray();
    }
    
    if (bestOf.hasValue()) obj["best_of"] = bestOf.getValue();
    if (echo.hasValue()) obj["echo"] = echo.getValue();
    if (frequencyPenalty.hasValue()) obj["frequency_penalty"] = frequencyPenalty.getValue();
    if (logitBias.hasValue()) obj["logit_bias"] = logitBias.getValue();
    if (logprobs.hasValue()) obj["logprobs"] = logprobs.getValue();
    if (maxTokens.hasValue()) obj["max_tokens"] = maxTokens.getValue();
    if (n.hasValue()) obj["n"] = n.getValue();
    if (presencePenalty.hasValue()) obj["presence_penalty"] = presencePenalty.getValue();
    if (seed.hasValue()) obj["seed"] = seed.getValue();
    
    if (stop.hasValue()) {
        QVariant stopValue = stop.getValue();
        if (stopValue.type() == QVariant::String) {
            obj["stop"] = stopValue.toString();
        } else if (stopValue.type() == QVariant::StringList) {
            QJsonArray arr;
            for (const QString& s : stopValue.toStringList()) {
                arr.append(s);
            }
            obj["stop"] = arr;
        }
    }
    
    if (stream.hasValue()) obj["stream"] = stream.getValue();
    if (streamOptions.hasValue()) obj["stream_options"] = streamOptions.getValue().toJson();
    if (suffix.hasValue()) obj["suffix"] = suffix.getValue();
    if (temperature.hasValue()) obj["temperature"] = temperature.getValue();
    if (topP.hasValue()) obj["top_p"] = topP.getValue();
    if (user.hasValue()) obj["user"] = user.getValue();
    
    return obj;
}

QCompletions::QCompletions(QOpenAIBaseClient* client, QObject* parent)
    : QObject(parent), m_client(client)
{
}

QFuture<CompletionResponse> QCompletions::create(const CompletionCreateParams& params,
                                                  const RequestOptions& options)
{
    return m_client->post<CompletionResponse>("/completions", params.toJson(), options);
}

// ============================================================================
// Embeddings Implementation
// ============================================================================

QJsonObject EmbeddingCreateParams::toJson() const
{
    QJsonObject obj;
    obj["model"] = model;
    
    // Handle input (can be string, array of strings, or array of ints)
    if (input.type() == QVariant::String) {
        obj["input"] = input.toString();
    } else if (input.type() == QVariant::StringList) {
        QJsonArray arr;
        for (const QString& s : input.toStringList()) {
            arr.append(s);
        }
        obj["input"] = arr;
    } else if (input.canConvert<QJsonArray>()) {
        obj["input"] = input.toJsonArray();
    }
    
    if (encodingFormat.hasValue()) obj["encoding_format"] = encodingFormat.getValue();
    if (dimensions.hasValue()) obj["dimensions"] = dimensions.getValue();
    if (user.hasValue()) obj["user"] = user.getValue();
    
    return obj;
}

QEmbeddings::QEmbeddings(QOpenAIBaseClient* client, QObject* parent)
    : QObject(parent), m_client(client)
{
}

QFuture<EmbeddingResponse> QEmbeddings::create(const EmbeddingCreateParams& params,
                                                const RequestOptions& options)
{
    return m_client->post<EmbeddingResponse>("/embeddings", params.toJson(), options);
}

QFuture<EmbeddingResponse> QEmbeddings::create(const QString& model, const QString& input,
                                                const RequestOptions& options)
{
    EmbeddingCreateParams params;
    params.model = model;
    params.input = input;
    return create(params, options);
}

// ============================================================================
// Models Implementation
// ============================================================================

QModels::QModels(QOpenAIBaseClient* client, QObject* parent)
    : QObject(parent), m_client(client)
{
}

QFuture<ModelListResponse> QModels::list(const RequestOptions& options)
{
    return m_client->get<ModelListResponse>("/models", Query(), options);
}

QFuture<OpenAIResponse> QModels::retrieve(const QString& model, const RequestOptions& options)
{
    return m_client->get<OpenAIResponse>(QString("/models/%1").arg(model), Query(), options);
}

QFuture<DeleteResponse> QModels::deleteModel(const QString& model, const RequestOptions& options)
{
    return m_client->deleteResource<DeleteResponse>(QString("/models/%1").arg(model), options);
}

// ============================================================================
// Files Implementation
// ============================================================================

QFiles::QFiles(QOpenAIBaseClient* client, QObject* parent)
    : QObject(parent), m_client(client)
{
}

QFuture<FileListResponse> QFiles::list(const QString& purpose, const RequestOptions& options)
{
    Query query;
    if (!purpose.isEmpty()) {
        query["purpose"] = purpose;
    }
    return m_client->get<FileListResponse>("/files", query, options);
}

QFuture<FileResponse> QFiles::create(const QByteArray& fileContent,
                                     const QString& filename,
                                     const QString& purpose,
                                     const RequestOptions& options)
{
    // TODO: Implement multipart/form-data upload
    // This requires a different implementation approach
    QPromise<FileResponse> promise;
    promise.setException(std::make_exception_ptr(
        OpenAIError("File upload not yet implemented")
    ));
    promise.finish();
    return promise.future();
}

QFuture<FileResponse> QFiles::retrieve(const QString& fileId, const RequestOptions& options)
{
    return m_client->get<FileResponse>(QString("/files/%1").arg(fileId), Query(), options);
}

QFuture<DeleteResponse> QFiles::deleteFile(const QString& fileId, const RequestOptions& options)
{
    return m_client->deleteResource<DeleteResponse>(QString("/files/%1").arg(fileId), options);
}

QFuture<QByteArray> QFiles::content(const QString& fileId, const RequestOptions& options)
{
    // TODO: Implement binary content download
    QPromise<QByteArray> promise;
    promise.setException(std::make_exception_ptr(
        OpenAIError("File content download not yet implemented")
    ));
    promise.finish();
    return promise.future();
}

// ============================================================================
// Images Implementation
// ============================================================================

QJsonObject ImageGenerateParams::toJson() const
{
    QJsonObject obj;
    obj["prompt"] = prompt;
    
    if (model.hasValue()) obj["model"] = model.getValue();
    if (n.hasValue()) obj["n"] = n.getValue();
    if (quality.hasValue()) obj["quality"] = quality.getValue();
    if (responseFormat.hasValue()) obj["response_format"] = responseFormat.getValue();
    if (size.hasValue()) obj["size"] = size.getValue();
    if (style.hasValue()) obj["style"] = style.getValue();
    if (user.hasValue()) obj["user"] = user.getValue();
    
    return obj;
}

QImages::QImages(QOpenAIBaseClient* client, QObject* parent)
    : QObject(parent), m_client(client)
{
}

QFuture<ImageResponse> QImages::generate(const ImageGenerateParams& params,
                                         const RequestOptions& options)
{
    return m_client->post<ImageResponse>("/images/generations", params.toJson(), options);
}

QFuture<ImageResponse> QImages::edit(const QByteArray& image,
                                     const QString& prompt,
                                     const QByteArray& mask,
                                     const RequestOptions& options)
{
    // TODO: Implement multipart/form-data upload for image edit
    QPromise<ImageResponse> promise;
    promise.setException(std::make_exception_ptr(
        OpenAIError("Image edit not yet implemented")
    ));
    promise.finish();
    return promise.future();
}

QFuture<ImageResponse> QImages::createVariation(const QByteArray& image,
                                                int n,
                                                const QString& size,
                                                const RequestOptions& options)
{
    // TODO: Implement multipart/form-data upload for image variation
    QPromise<ImageResponse> promise;
    promise.setException(std::make_exception_ptr(
        OpenAIError("Image variation not yet implemented")
    ));
    promise.finish();
    return promise.future();
}

// ============================================================================
// Moderations Implementation
// ============================================================================

QJsonObject ModerationCreateParams::toJson() const
{
    QJsonObject obj;
    
    if (input.type() == QVariant::String) {
        obj["input"] = input.toString();
    } else if (input.type() == QVariant::StringList) {
        QJsonArray arr;
        for (const QString& s : input.toStringList()) {
            arr.append(s);
        }
        obj["input"] = arr;
    }
    
    if (model.hasValue()) obj["model"] = model.getValue();
    
    return obj;
}

QModerations::QModerations(QOpenAIBaseClient* client, QObject* parent)
    : QObject(parent), m_client(client)
{
}

QFuture<OpenAIResponse> QModerations::create(const ModerationCreateParams& params,
                                              const RequestOptions& options)
{
    return m_client->post<OpenAIResponse>("/moderations", params.toJson(), options);
}

QFuture<OpenAIResponse> QModerations::create(const QString& input,
                                              const RequestOptions& options)
{
    ModerationCreateParams params;
    params.input = input;
    return create(params, options);
}

} // namespace QOpenAI
