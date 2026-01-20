#include "qopenai.h"
#include <QProcessEnvironment>

namespace QOpenAI {

QOpenAI::QOpenAI(const OpenAIConfig& config, QObject* parent)
    : QOpenAIBaseClient(config, parent)
{
    initializeResources();
}

QOpenAI::QOpenAI(const QString& apiKey, QObject* parent)
    : QOpenAIBaseClient(OpenAIConfig(), parent)
{
    // Set API key from parameter or environment
    if (!apiKey.isEmpty()) {
        m_config.apiKey = apiKey;
    } else {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        // m_config.apiKey = env.value("OPENAI_API_KEY");
    }
    
    // Load other config from environment
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains("OPENAI_ORG_ID")) {
        m_config.organization = env.value("OPENAI_ORG_ID");
    }
    if (env.contains("OPENAI_PROJECT_ID")) {
        m_config.project = env.value("OPENAI_PROJECT_ID");
    }
    if (env.contains("OPENAI_BASE_URL")) {
        m_config.baseUrl = QUrl(env.value("OPENAI_BASE_URL"));
    }
    
    // Validate API key
    if (m_config.apiKey.isEmpty()) {
        throw OpenAIError(
            "The api_key must be set either by passing it to the constructor "
            "or by setting the OPENAI_API_KEY environment variable"
        );
    }
    
    initializeResources();
}

QOpenAI::~QOpenAI()
{
}

void QOpenAI::initializeResources()
{
    m_chat = new QChat(this, this);
    m_completions = new QCompletions(this, this);
    m_embeddings = new QEmbeddings(this, this);
    m_models = new QModels(this, this);
    m_files = new QFiles(this, this);
    m_images = new QImages(this, this);
    m_moderations = new QModerations(this, this);
}

} // namespace QOpenAI
