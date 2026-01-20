/**
 * Embeddings Example
 * 
 * 展示如何使用 Embeddings API 生成文本向量
 */

#include "../qopenai.h"
#include <QCoreApplication>
#include <QDebug>
#include <cmath>

// using namespace QOpenAI;

// 计算余弦相似度
float cosineSimilarity(const QList<float>& a, const QList<float>& b)
{
    if (a.size() != b.size() || a.isEmpty()) {
        return 0.0f;
    }
    
    float dotProduct = 0.0f;
    float normA = 0.0f;
    float normB = 0.0f;
    
    for (int i = 0; i < a.size(); ++i) {
        dotProduct += a[i] * b[i];
        normA += a[i] * a[i];
        normB += b[i] * b[i];
    }
    
    return dotProduct / (std::sqrt(normA) * std::sqrt(normB));
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== QOpenAI Embeddings Example ===\n";
    
    try {
        QOpenAI::QOpenAI client;
        
        // 示例 1: 单个文本的嵌入
        qDebug() << "示例 1: 生成单个文本的向量";
        qDebug() << "-----------------------------------";
        
        auto future1 = client.embeddings()->create(
            "text-embedding-3-small", 
            "人工智能正在改变世界"
        );
        
        future1.then([&client, &app](QOpenAI::EmbeddingResponse response) {
            auto embeddings = response.embeddings();
            if (!embeddings.isEmpty()) {
                qDebug() << "向量维度:" << embeddings.first().size();
                qDebug() << "前 5 个值:" 
                         << embeddings.first().mid(0, 5);
            }
            qDebug() << "Token 使用:" << response.usage().totalTokens;
            
            // 示例 2: 批量生成和相似度计算
            qDebug() << "\n\n示例 2: 批量生成和相似度计算";
            qDebug() << "-----------------------------------";
            
            QOpenAI::EmbeddingCreateParams params;
            params.model = "text-embedding-3-small";
            params.input = QStringList{
                "人工智能正在改变世界",
                "AI 技术发展迅速",
                "今天天气真好"
            };
            
            auto future2 = client.embeddings()->create(params);
            
            future2.then([&app](QOpenAI::EmbeddingResponse response) {
                auto embeddings = response.embeddings();
                
                if (embeddings.size() >= 3) {
                    qDebug() << "生成了" << embeddings.size() << "个向量\n";
                    
                    // 计算相似度
                    float sim1 = cosineSimilarity(embeddings[0], embeddings[1]);
                    float sim2 = cosineSimilarity(embeddings[0], embeddings[2]);
                    float sim3 = cosineSimilarity(embeddings[1], embeddings[2]);
                    
                    qDebug() << "相似度分析:";
                    qDebug() << QString("'人工智能正在改变世界' vs 'AI 技术发展迅速': %1")
                                .arg(sim1, 0, 'f', 4);
                    qDebug() << QString("'人工智能正在改变世界' vs '今天天气真好': %1")
                                .arg(sim2, 0, 'f', 4);
                    qDebug() << QString("'AI 技术发展迅速' vs '今天天气真好': %1")
                                .arg(sim3, 0, 'f', 4);
                    
                    qDebug() << "\n观察: 语义相关的文本有更高的相似度";
                }
                
                qDebug() << "\nToken 使用:" << response.usage().totalTokens;
                
                app.quit();
            }).onFailed([&app](const std::exception& e) {
                qDebug() << "示例 2 错误:" << e.what();
                app.exit(1);
            });
            
        }).onFailed([&app](const std::exception& e) {
            qDebug() << "示例 1 错误:" << e.what();
            app.exit(1);
        });
        
    } catch (const QOpenAI::OpenAIError& e) {
        qDebug() << "初始化错误:" << e.message();
        return 1;
    }
    
    return app.exec();
}
