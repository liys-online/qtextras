/**
 * Advanced Chat Example
 * 
 * 展示 Chat Completions API 的高级功能：
 * - 系统消息
 * - 温度和其他参数控制
 * - JSON 模式输出
 * - 多轮对话
 */

#include "../qopenai.h"
#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>

// using namespace QOpenAI;

void example1_SystemMessage(QOpenAI::QOpenAI& client);
void example2_JsonMode(QOpenAI::QOpenAI& client);
void example3_MultiTurn(QOpenAI::QOpenAI& client);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== QOpenAI Advanced Chat Examples ===\n";
    
    try {
        QOpenAI::QOpenAI client;
        
        // 示例 1: 使用系统消息
        qDebug() << "示例 1: 系统消息和参数控制";
        qDebug() << "-----------------------------------";
        example1_SystemMessage(client);
        
        // 等待第一个完成后再执行下一个
        QTimer::singleShot(5000, [&client]() {
            qDebug() << "\n\n示例 2: JSON 模式输出";
            qDebug() << "-----------------------------------";
            example2_JsonMode(client);
        });
        
        QTimer::singleShot(10000, [&client, &app]() {
            qDebug() << "\n\n示例 3: 多轮对话";
            qDebug() << "-----------------------------------";
            example3_MultiTurn(client);
            
            // 等待最后一个完成
            QTimer::singleShot(5000, &app, &QCoreApplication::quit);
        });
        
    } catch (const QOpenAI::OpenAIError& e) {
        qDebug() << "错误:" << e.message();
        return 1;
    }
    
    return app.exec();
}

void example1_SystemMessage(QOpenAI::QOpenAI& client)
{
    QOpenAI::ChatCompletionCreateParams params;
    params.model = "gpt-4o-mini";
    
    // 设置系统消息定义助手行为
    QOpenAI::ChatMessage systemMsg;
    systemMsg.role = QOpenAI::MessageRole::System;
    systemMsg.content = "你是一个专业的 C++ 编程专家，擅长用简洁的语言解释复杂的概念。";
    params.messages.append(systemMsg);
    
    // 用户问题
    QOpenAI::ChatMessage userMsg;
    userMsg.role = QOpenAI::MessageRole::User;
    userMsg.content = "什么是 RAII？请用一句话解释。";
    params.messages.append(userMsg);
    
    // 设置参数
    params.temperature = QOpenAI::Omit<float>(0.3f);  // 更确定的输出
    params.maxTokens = QOpenAI::Omit<int>(100);       // 限制长度
    
    auto future = client.chat()->completions()->create(params);
    
    future.then([](QOpenAI::ChatCompletionResponse response) {
        qDebug() << "问题: 什么是 RAII？";
        qDebug() << "回答:" << response.content();
        qDebug() << "Tokens 使用:" << response.usage().totalTokens;
    }).onFailed([](const std::exception& e) {
        qDebug() << "错误:" << e.what();
    });
}

void example2_JsonMode(QOpenAI::QOpenAI& client)
{
    QOpenAI::ChatCompletionCreateParams params;
    params.model = "gpt-4o-mini";
    
    QOpenAI::ChatMessage userMsg;
    userMsg.role = QOpenAI::MessageRole::User;
    userMsg.content = "生成一个包含三个编程语言的 JSON 对象，每个语言包含 name 和 year 字段";
    params.messages.append(userMsg);
    
    // 设置响应格式为 JSON
    QOpenAI::ResponseFormat format;
    format.type = QOpenAI::ResponseFormatType::JsonObject;
    params.responseFormat = QOpenAI::Omit<QOpenAI::ResponseFormat>(format);
    
    auto future = client.chat()->completions()->create(params);
    
    future.then([](QOpenAI::ChatCompletionResponse response) {
        qDebug() << "JSON 响应:";
        QString jsonStr = response.content();
        QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
        qDebug() << doc.toJson(QJsonDocument::Indented);
    }).onFailed([](const std::exception& e) {
        qDebug() << "错误:" << e.what();
    });
}

void example3_MultiTurn(QOpenAI::QOpenAI& client)
{
    QOpenAI::ChatCompletionCreateParams params;
    params.model = "gpt-4o-mini";
    
    // 第一轮：用户提问
    QOpenAI::ChatMessage msg1;
    msg1.role = QOpenAI::MessageRole::User;
    msg1.content = "我想学习 Qt 框架";
    params.messages.append(msg1);
    
    // 第二轮：助手回复（模拟）
    QOpenAI::ChatMessage msg2;
    msg2.role = QOpenAI::MessageRole::Assistant;
    msg2.content = "Qt 是一个跨平台的 C++ 框架，适合开发桌面和移动应用。";
    params.messages.append(msg2);
    
    // 第三轮：用户追问
    QOpenAI::ChatMessage msg3;
    msg3.role = QOpenAI::MessageRole::User;
    msg3.content = "有哪些学习资源推荐？";
    params.messages.append(msg3);
    
    auto future = client.chat()->completions()->create(params);
    
    future.then([](QOpenAI::ChatCompletionResponse response) {
        qDebug() << "对话历史:";
        qDebug() << "用户: 我想学习 Qt 框架";
        qDebug() << "助手: Qt 是一个跨平台的 C++ 框架...";
        qDebug() << "用户: 有哪些学习资源推荐？";
        qDebug() << "\n助手回复:";
        qDebug() << response.content();
    }).onFailed([](const std::exception& e) {
        qDebug() << "错误:" << e.what();
    });
}
