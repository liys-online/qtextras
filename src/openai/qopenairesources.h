#ifndef QOPENAIRESOURCES_H
#define QOPENAIRESOURCES_H

#include "openai_global.h"
#include "qopenaitypes.h"
#include "qopenairesponse.h"
#include <QObject>
#include <QFuture>
#include <QJsonObject>
#include <QJsonArray>

namespace QOpenAI {

class QOpenAIBaseClient;

// ============================================================================
// Completions (Legacy API)
// ============================================================================

struct QTEXTRAS_OPENAI_EXPORT CompletionCreateParams {
    QString model;
    QVariant prompt; // Can be QString, QStringList, or array of ints
    
    Omit<int> bestOf;
    Omit<bool> echo;
    Omit<float> frequencyPenalty;
    Omit<QJsonObject> logitBias;
    Omit<int> logprobs;
    Omit<int> maxTokens;
    Omit<int> n;
    Omit<float> presencePenalty;
    Omit<int> seed;
    Omit<QVariant> stop;
    Omit<bool> stream;
    Omit<StreamOptions> streamOptions;
    Omit<QString> suffix;
    Omit<float> temperature;
    Omit<float> topP;
    Omit<QString> user;
    
    QJsonObject toJson() const;
};

class QTEXTRAS_OPENAI_EXPORT QCompletions : public QObject
{
    Q_OBJECT
public:
    explicit QCompletions(QOpenAIBaseClient* client, QObject* parent = nullptr);
    
    QFuture<CompletionResponse> create(const CompletionCreateParams& params,
                                       const RequestOptions& options = RequestOptions());

private:
    QOpenAIBaseClient* m_client;
};

// ============================================================================
// Embeddings
// ============================================================================

struct QTEXTRAS_OPENAI_EXPORT EmbeddingCreateParams {
    QString model;
    QVariant input; // Can be QString, QStringList, or array of ints
    
    Omit<QString> encodingFormat; // "float" or "base64"
    Omit<int> dimensions;
    Omit<QString> user;
    
    QJsonObject toJson() const;
};

class QTEXTRAS_OPENAI_EXPORT QEmbeddings : public QObject
{
    Q_OBJECT
public:
    explicit QEmbeddings(QOpenAIBaseClient* client, QObject* parent = nullptr);
    
    QFuture<EmbeddingResponse> create(const EmbeddingCreateParams& params,
                                      const RequestOptions& options = RequestOptions());
    
    // Convenience method
    QFuture<EmbeddingResponse> create(const QString& model, const QString& input,
                                      const RequestOptions& options = RequestOptions());

private:
    QOpenAIBaseClient* m_client;
};

// ============================================================================
// Models
// ============================================================================

class QTEXTRAS_OPENAI_EXPORT QModels : public QObject
{
    Q_OBJECT
public:
    explicit QModels(QOpenAIBaseClient* client, QObject* parent = nullptr);
    
    // List all models
    QFuture<ModelListResponse> list(const RequestOptions& options = RequestOptions());
    
    // Retrieve a specific model
    QFuture<OpenAIResponse> retrieve(const QString& model,
                                     const RequestOptions& options = RequestOptions());
    
    // Delete a fine-tuned model
    QFuture<DeleteResponse> deleteModel(const QString& model,
                                        const RequestOptions& options = RequestOptions());

private:
    QOpenAIBaseClient* m_client;
};

// ============================================================================
// Files
// ============================================================================

class QTEXTRAS_OPENAI_EXPORT QFiles : public QObject
{
    Q_OBJECT
public:
    explicit QFiles(QOpenAIBaseClient* client, QObject* parent = nullptr);
    
    // List files
    QFuture<FileListResponse> list(const QString& purpose = QString(),
                                   const RequestOptions& options = RequestOptions());
    
    // Upload a file
    QFuture<FileResponse> create(const QByteArray& fileContent,
                                 const QString& filename,
                                 const QString& purpose,
                                 const RequestOptions& options = RequestOptions());
    
    // Retrieve a file
    QFuture<FileResponse> retrieve(const QString& fileId,
                                   const RequestOptions& options = RequestOptions());
    
    // Delete a file
    QFuture<DeleteResponse> deleteFile(const QString& fileId,
                                       const RequestOptions& options = RequestOptions());
    
    // Download file content
    QFuture<QByteArray> content(const QString& fileId,
                                const RequestOptions& options = RequestOptions());

private:
    QOpenAIBaseClient* m_client;
};

// ============================================================================
// Images
// ============================================================================

struct QTEXTRAS_OPENAI_EXPORT ImageGenerateParams {
    QString prompt;
    Omit<QString> model; // Default: "dall-e-2"
    Omit<int> n;
    Omit<QString> quality; // "standard" or "hd"
    Omit<QString> responseFormat; // "url" or "b64_json"
    Omit<QString> size; // "256x256", "512x512", "1024x1024", "1792x1024", "1024x1792"
    Omit<QString> style; // "vivid" or "natural"
    Omit<QString> user;
    
    QJsonObject toJson() const;
};

class QTEXTRAS_OPENAI_EXPORT QImages : public QObject
{
    Q_OBJECT
public:
    explicit QImages(QOpenAIBaseClient* client, QObject* parent = nullptr);
    
    // Generate images
    QFuture<ImageResponse> generate(const ImageGenerateParams& params,
                                    const RequestOptions& options = RequestOptions());
    
    // Edit an image
    QFuture<ImageResponse> edit(const QByteArray& image,
                                const QString& prompt,
                                const QByteArray& mask = QByteArray(),
                                const RequestOptions& options = RequestOptions());
    
    // Create image variations
    QFuture<ImageResponse> createVariation(const QByteArray& image,
                                           int n = 1,
                                           const QString& size = QString(),
                                           const RequestOptions& options = RequestOptions());

private:
    QOpenAIBaseClient* m_client;
};

// ============================================================================
// Moderations
// ============================================================================

struct QTEXTRAS_OPENAI_EXPORT ModerationCreateParams {
    QVariant input; // Can be QString or QStringList
    Omit<QString> model; // Default: "text-moderation-latest"
    
    QJsonObject toJson() const;
};

class QTEXTRAS_OPENAI_EXPORT QModerations : public QObject
{
    Q_OBJECT
public:
    explicit QModerations(QOpenAIBaseClient* client, QObject* parent = nullptr);
    
    QFuture<OpenAIResponse> create(const ModerationCreateParams& params,
                                   const RequestOptions& options = RequestOptions());
    
    // Convenience method
    QFuture<OpenAIResponse> create(const QString& input,
                                   const RequestOptions& options = RequestOptions());

private:
    QOpenAIBaseClient* m_client;
};

} // namespace QOpenAI

#endif // QOPENAIRESOURCES_H
