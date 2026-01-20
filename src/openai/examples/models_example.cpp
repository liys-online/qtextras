/**
 * Models Example
 * 
 * 展示如何列出和检索模型信息
 */

#include "../qopenai.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>

// using namespace QOpenAI;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== QOpenAI Models Example ===\n";
    
    // try {
        QOpenAI::QOpenAI client;
        
        // 列出所有模型
        qDebug() << "正在获取可用模型列表...\n";
        
        auto listFuture = client.models()->list();
        
        listFuture.then([&client, &app](QOpenAI::ModelListResponse response) {
            auto models = response.models();
            
            qDebug() << "找到" << models.size() << "个模型\n";
            
            // 按类型分组显示
            QStringList gptModels;
            QStringList embeddingModels;
            QStringList dalleModels;
            QStringList otherModels;
            
            for (const auto& model : models) {
                if (model.id.startsWith("gpt")) {
                    gptModels.append(model.id);
                } else if (model.id.contains("embedding")) {
                    embeddingModels.append(model.id);
                } else if (model.id.contains("dall-e")) {
                    dalleModels.append(model.id);
                } else {
                    otherModels.append(model.id);
                }
            }
            
            qDebug() << "=== GPT 模型 (" << gptModels.size() << ") ===";
            for (const QString& model : gptModels) {
                qDebug() << "  -" << model;
            }
            
            qDebug() << "\n=== Embedding 模型 (" << embeddingModels.size() << ") ===";
            for (const QString& model : embeddingModels) {
                qDebug() << "  -" << model;
            }
            
            qDebug() << "\n=== DALL-E 模型 (" << dalleModels.size() << ") ===";
            for (const QString& model : dalleModels) {
                qDebug() << "  -" << model;
            }
            
            if (!otherModels.isEmpty()) {
                qDebug() << "\n=== 其他模型 (" << otherModels.size() << ") ===";
                for (const QString& model : otherModels) {
                    qDebug() << "  -" << model;
                }
            }
            
            // 获取特定模型的详细信息
            if (!models.isEmpty()) {
                QString targetModel = "gpt-4o";
                
                // 查找目标模型
                bool found = false;
                for (const auto& model : models) {
                    if (model.id == targetModel) {
                        found = true;
                        break;
                    }
                }
                
                if (found) {
                    qDebug() << "\n\n正在获取模型详细信息:" << targetModel;
                    qDebug() << "-----------------------------------";
                    
                    auto retrieveFuture = client.models()->retrieve(targetModel);
                    
                    retrieveFuture.then([&app](QOpenAI::OpenAIResponse response) {
                        QJsonObject data = response.raw();
                        
                        qDebug() << "ID:" << data["id"].toString();
                        qDebug() << "对象类型:" << data["object"].toString();
                        
                        qint64 created = data["created"].toVariant().toLongLong();
                        QDateTime dt = QDateTime::fromSecsSinceEpoch(created);
                        qDebug() << "创建时间:" << dt.toString(Qt::ISODate);
                        
                        qDebug() << "拥有者:" << data["owned_by"].toString();
                        
                        app.quit();
                    }).onFailed([&app](const std::exception& e) {
                        qDebug() << "获取模型详情失败:" << e.what();
                        app.exit(1);
                    });
                } else {
                    qDebug() << "\n模型" << targetModel << "不可用";
                    app.quit();
                }
            } else {
                app.quit();
            }
            
        }).onFailed([&app](const std::exception& e) {
            try {
                std::rethrow_exception(std::current_exception());
            } catch (const QOpenAI::AuthenticationError& err) {
                qDebug() << "认证错误:" << err.message();
                qDebug() << "请设置环境变量 OPENAI_API_KEY";
            } catch (const QOpenAI::OpenAIError& err) {
                qDebug() << "错误:" << err.message();
            } catch (const std::exception& err) {
                qDebug() << "未知错误:" << err.what();
            }
            app.exit(1);
        });
        
    // } catch (const QOpenAI::OpenAIError& e) {
    //     qDebug() << "初始化错误:" << e.message();
    //     return 1;
    // }
    
    return app.exec();
}
