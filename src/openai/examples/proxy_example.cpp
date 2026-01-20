/**
 * Proxy Configuration Example
 * 
 * 展示如何配置和使用代理访问 OpenAI API
 */

#include "../qopenai.h"
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkProxy>

// using namespace QOpenAI;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== QOpenAI Proxy Configuration Example ===\n";
    
    // try {
        // 配置代理
        QOpenAI::OpenAIConfig config;
        // config.apiKey = qgetenv("OPENAI_API_KEY"); // 从环境变量读取
        
        // 示例 1: Clash 客户端代理（推荐）
        qDebug() << "配置 Clash HTTP 代理...";
        QNetworkProxy clashProxy;
        clashProxy.setType(QNetworkProxy::HttpProxy);
        clashProxy.setHostName("127.0.0.1");  // Clash 本地地址
        clashProxy.setPort(7890);              // Clash 默认 HTTP 端口
        
        config.proxy = clashProxy;
        config.useProxy = true;
        
        // 如果使用其他代理服务器
        // QNetworkProxy customProxy;
        // customProxy.setType(QNetworkProxy::HttpProxy);
        // customProxy.setHostName("proxy.example.com");
        // customProxy.setPort(8080);
        // customProxy.setUser("username");  // 如果需要认证
        // customProxy.setPassword("password");
        // config.proxy = customProxy;
        // config.useProxy = true;
        
        QOpenAI::QOpenAI client(config);
        
        qDebug() << "通过代理发送请求...\n";
        
        QList<QOpenAI::ChatMessage> messages;
        QOpenAI::ChatMessage msg;
        msg.role = QOpenAI::MessageRole::User;
        msg.content = "你好，这是一个测试";
        messages.append(msg);
        
        auto future = client.chat()->completions()->create("gpt-4o-mini", messages);
        
        future.then([&app](QOpenAI::ChatCompletionResponse response) {
            qDebug() << "\n=== 成功通过代理获得响应 ===";
            qDebug() << "模型:" << response.model();
            qDebug() << "内容:" << response.content();
            qDebug() << "Tokens:" << response.usage().totalTokens;
            
            app.quit();
        }).onFailed([&app](const std::exception& e) {
            qDebug() << "\n=== 错误 ===";
            
            try {
                std::rethrow_exception(std::current_exception());
            } catch (const QOpenAI::APIConnectionError& err) {
                qDebug() << "连接错误（可能是代理配置问题）:" << err.message();
                qDebug() << "请检查:";
                qDebug() << "1. 代理地址和端口是否正确";
                qDebug() << "2. 代理是否需要认证";
                qDebug() << "3. 代理是否允许 HTTPS 连接";
            } catch (const QOpenAI::OpenAIError& err) {
                qDebug() << "OpenAI 错误:" << err.message();
            }
            
            app.exit(1);
        });
        
    // } catch (const QOpenAI::OpenAIError& e) {
    //     qDebug() << "初始化错误:" << e.message();
    //     return 1;
    // }
    
    return app.exec();
}

/*
 * 常见代理配置示例：
 * 
 * 1. Clash 客户端（推荐）
 *    HTTP 代理：
 *    QNetworkProxy proxy;
 *    proxy.setType(QNetworkProxy::HttpProxy);
 *    proxy.setHostName("127.0.0.1");
 *    proxy.setPort(7890);  // Clash 默认 HTTP 端口
 *    
 *    SOCKS5 代理：
 *    QNetworkProxy proxy;
 *    proxy.setType(QNetworkProxy::Socks5Proxy);
 *    proxy.setHostName("127.0.0.1");
 *    proxy.setPort(7891);  // Clash 默认 SOCKS5 端口
 * 
 * 2. V2Ray/V2RayN
 *    QNetworkProxy proxy;
 *    proxy.setType(QNetworkProxy::Socks5Proxy);
 *    proxy.setHostName("127.0.0.1");
 *    proxy.setPort(10808);  // V2Ray 默认端口
 * 
 * 3. 企业/自定义 HTTP 代理（需要认证）
 *    QNetworkProxy proxy;
 *    proxy.setType(QNetworkProxy::HttpProxy);
 *    proxy.setHostName("proxy.company.com");
 *    proxy.setPort(8080);
 *    proxy.setUser("username");
 *    proxy.setPassword("password");
 * 
 * 4. 系统代理设置
 *    QNetworkProxyFactory::setUseSystemConfiguration(true);
 *    // 然后不需要在 config 中设置 useProxy
 * 
 * 5. 取消代理
 *    config.useProxy = false;
 * 
 * 注意：
 * - Clash 需要在系统托盘运行并启用代理模式
 * - 确认 Clash 配置中的端口号与代码中设置一致
 * - 可以在 Clash 界面的「常规」设置中查看端口
 */
