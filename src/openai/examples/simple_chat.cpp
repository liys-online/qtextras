/**
 * Simple Chat Example
 * 
 * 这个示例展示如何使用 QOpenAI 进行简单的对话。
 * 类似于 Python SDK 的基础用法。
 */

#include "../qopenai.h"
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

// using namespace QOpenAI;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== QOpenAI Simple Chat Example ===\n";
    
    // 创建客户端（从环境变量 OPENAI_API_KEY 读取 API key）
    QOpenAI::QOpenAI client;

    qDebug() << "发送消息: 你好，请用一句话介绍你自己\n";

    // 构建消息列表
    QList<QOpenAI::ChatMessage> messages;
    QOpenAI::ChatMessage userMsg;
    userMsg.role = QOpenAI::MessageRole::User;
    userMsg.content = "你好，请用一句话介绍你自己";
    messages.append(userMsg);

    // 调用 API
    auto future = client.chat()->completions()->create("deepseek-chat", messages);

    // 处理响应
    future.then([&app](QOpenAI::ChatCompletionResponse response) {
        qDebug() << "\n=== 响应 ===";
        qDebug() << "模型:" << response.model();
        qDebug() << "内容:" << response.content();
        qDebug() << "\n=== Token 使用情况 ===";
        qDebug() << "输入 tokens:" << response.usage().promptTokens;
        qDebug() << "输出 tokens:" << response.usage().completionTokens;
        qDebug() << "总计 tokens:" << response.usage().totalTokens;
        qDebug() << "\n请求 ID:" << response.requestId();

        app.quit();
    }).onFailed([&app](const std::exception& e) {
        qDebug() << "\n=== 错误 ===";

        try {
            std::rethrow_exception(std::current_exception());
        } catch (const QOpenAI::AuthenticationError& err) {
            qDebug() << "认证错误:" << err.message();
            qDebug() << "请设置环境变量 OPENAI_API_KEY";
        } catch (const QOpenAI::RateLimitError& err) {
            qDebug() << "速率限制错误:" << err.message();
        } catch (const QOpenAI::APIConnectionError& err) {
            qDebug() << "连接错误:" << err.message();
        } catch (const QOpenAI::APITimeoutError& err) {
            qDebug() << "超时错误:" << err.message();
        } catch (const QOpenAI::OpenAIError& err) {
            qDebug() << "OpenAI 错误:" << err.message();
        } catch (const std::exception& err) {
            qDebug() << "未知错误:" << err.what();
        }

        app.exit(1);
    });

    
    return app.exec();
}
