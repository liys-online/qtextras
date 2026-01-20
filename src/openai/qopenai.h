#ifndef QOPENAI_H
#define QOPENAI_H

#include "openai_global.h"
#include "qopenaibaseclient.h"
#include "qopenaichat.h"
#include "qopenairesources.h"

namespace QOpenAI {

class QTEXTRAS_OPENAI_EXPORT QOpenAI : public QOpenAIBaseClient
{
    Q_OBJECT
public:
    explicit QOpenAI(const OpenAIConfig& config, QObject* parent = nullptr);
    explicit QOpenAI(const QString& apiKey = QString(), QObject* parent = nullptr);
    virtual ~QOpenAI();
    
    // Resource accessors
    QChat* chat() { return m_chat; }
    QCompletions* completions() { return m_completions; }
    QEmbeddings* embeddings() { return m_embeddings; }
    QModels* models() { return m_models; }
    QFiles* files() { return m_files; }
    QImages* images() { return m_images; }
    QModerations* moderations() { return m_moderations; }

private:
    void initializeResources();
    
    QChat* m_chat;
    QCompletions* m_completions;
    QEmbeddings* m_embeddings;
    QModels* m_models;
    QFiles* m_files;
    QImages* m_images;
    QModerations* m_moderations;
};

} // namespace QOpenAI

#endif // QOPENAI_H
