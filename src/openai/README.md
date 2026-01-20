# QOpenAI - Qt C++ OpenAI API Client

基于 OpenAI Python SDK 实现的 Qt C++ 版本，提供类型安全的 OpenAI API 访问。

## 功能特性

- ✅ 完整的类型定义和错误处理
- ✅ Chat Completions API
- ✅ Completions API (Legacy)
- ✅ Embeddings API
- ✅ Models API
- ✅ Files API
- ✅ Images API
- ✅ Moderations API
- ✅ 异步操作支持 (QFuture)
- ✅ 自动重试机制
- ✅ 超时配置
- ✅ 请求头自定义
- ⏳ 流式响应 (Streaming)
- ⏳ 多部分表单上传

## 安装

### 从源码构建

```bash
mkdir build && cd build
cmake ..
cmake --build .
cmake --install .
```

## 快速开始

### 基础用法

```cpp
#include <QOpenAI>
#include <QCoreApplication>
#include <QDebug>

using namespace QOpenAI;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    // 方式 1: 从环境变量读取 API key (OPENAI_API_KEY)
    QOpenAI client;
    
    // 方式 2: 直接传入 API key
    // QOpenAI client("your-api-key-here");
    
    // 方式 3: 使用完整配置
    // OpenAIConfig config;
    // config.apiKey = "your-api-key-here";
    // config.organization = "your-org-id";
    // QOpenAI client(config);
    
    return app.exec();
}
```

### Chat Completions

```cpp
#include <QOpenAI>
#include <QDebug>

using namespace QOpenAI;

// 简单对话
void simpleChatExample()
{
    QOpenAI client;
    
    QList<ChatMessage> messages;
    ChatMessage userMsg;
    userMsg.role = MessageRole::User;
    userMsg.content = "你好，请介绍一下你自己";
    messages.append(userMsg);
    
    auto future = client.chat()->completions()->create("gpt-4o", messages);
    future.then([](ChatCompletionResponse response) {
        qDebug() << "回复:" << response.content();
        qDebug() << "使用的 tokens:" << response.usage().totalTokens;
    }).onFailed([](const std::exception& e) {
        qDebug() << "错误:" << e.what();
    });
}

// 使用完整参数
void advancedChatExample()
{
    QOpenAI client;
    
    ChatCompletionCreateParams params;
    params.model = "gpt-4o";
    
    // 添加系统消息
    ChatMessage systemMsg;
    systemMsg.role = MessageRole::System;
    systemMsg.content = "你是一个专业的编程助手";
    params.messages.append(systemMsg);
    
    // 添加用户消息
    ChatMessage userMsg;
    userMsg.role = MessageRole::User;
    userMsg.content = "如何在 C++ 中使用智能指针？";
    params.messages.append(userMsg);
    
    // 设置可选参数
    params.temperature = Omit<float>(0.7f);
    params.maxTokens = Omit<int>(1000);
    params.topP = Omit<float>(1.0f);
    
    auto future = client.chat()->completions()->create(params);
    future.then([](ChatCompletionResponse response) {
        qDebug() << response.content();
    });
}

// JSON 模式输出
void jsonModeExample()
{
    QOpenAI client;
    
    ChatCompletionCreateParams params;
    params.model = "gpt-4o";
    
    ChatMessage userMsg;
    userMsg.role = MessageRole::User;
    userMsg.content = "生成一个包含姓名和年龄的 JSON 对象";
    params.messages.append(userMsg);
    
    // 设置响应格式为 JSON
    ResponseFormat format;
    format.type = ResponseFormatType::JsonObject;
    params.responseFormat = Omit<ResponseFormat>(format);
    
    auto future = client.chat()->completions()->create(params);
    future.then([](ChatCompletionResponse response) {
        qDebug() << "JSON 响应:" << response.content();
    });
}

// 多模态：图像输入
void visionExample()
{
    QOpenAI client;
    
    ChatMessage userMsg;
    userMsg.role = MessageRole::User;
    
    // 构建多模态内容
    QJsonArray contentArray;
    
    // 添加文本
    TextContent textContent;
    textContent.text = "这张图片里有什么？";
    contentArray.append(textContent.toJson());
    
    // 添加图像
    ImageContent imageContent;
    imageContent.imageUrl.url = "https://example.com/image.jpg";
    imageContent.imageUrl.detail = "high";
    contentArray.append(imageContent.toJson());
    
    userMsg.contentArray = contentArray;
    
    ChatCompletionCreateParams params;
    params.model = "gpt-4o";
    params.messages.append(userMsg);
    
    auto future = client.chat()->completions()->create(params);
    future.then([](ChatCompletionResponse response) {
        qDebug() << response.content();
    });
}
```

### Embeddings

```cpp
#include <QOpenAI>
#include <QDebug>

using namespace QOpenAI;

void embeddingsExample()
{
    QOpenAI client;
    
    // 方式 1: 简单用法
    auto future = client.embeddings()->create("text-embedding-3-small", 
                                               "Hello, world!");
    future.then([](EmbeddingResponse response) {
        auto embeddings = response.embeddings();
        if (!embeddings.isEmpty()) {
            qDebug() << "向量维度:" << embeddings.first().size();
        }
    });
    
    // 方式 2: 批量处理
    EmbeddingCreateParams params;
    params.model = "text-embedding-3-small";
    params.input = QStringList{"文本1", "文本2", "文本3"};
    
    auto batchFuture = client.embeddings()->create(params);
    batchFuture.then([](EmbeddingResponse response) {
        auto embeddings = response.embeddings();
        qDebug() << "生成了" << embeddings.size() << "个向量";
    });
}
```

### Models

```cpp
#include <QOpenAI>
#include <QDebug>

using namespace QOpenAI;

void modelsExample()
{
    QOpenAI client;
    
    // 列出所有模型
    auto listFuture = client.models()->list();
    listFuture.then([](ModelListResponse response) {
        auto models = response.models();
        qDebug() << "可用模型数量:" << models.size();
        for (const auto& model : models) {
            qDebug() << "模型:" << model.id;
        }
    });
    
    // 获取特定模型信息
    auto retrieveFuture = client.models()->retrieve("gpt-4o");
    retrieveFuture.then([](OpenAIResponse response) {
        qDebug() << "模型信息:" << response.raw();
    });
}
```

### Images

```cpp
#include <QOpenAI>
#include <QDebug>

using namespace QOpenAI;

void imageGenerationExample()
{
    QOpenAI client;
    
    ImageGenerateParams params;
    params.prompt = "一只可爱的小猫在弹钢琴";
    params.model = Omit<QString>("dall-e-3");
    params.size = Omit<QString>("1024x1024");
    params.quality = Omit<QString>("hd");
    params.n = Omit<int>(1);
    
    auto future = client.images()->generate(params);
    future.then([](ImageResponse response) {
        auto urls = response.urls();
        if (!urls.isEmpty()) {
            qDebug() << "图像 URL:" << urls.first();
        }
    });
}
```

### Files

```cpp
#include <QOpenAI>
#include <QDebug>

using namespace QOpenAI;

void filesExample()
{
    QOpenAI client;
    
    // 列出文件
    auto listFuture = client.files()->list();
    listFuture.then([](FileListResponse response) {
        auto files = response.files();
        for (const auto& file : files) {
            qDebug() << "文件:" << file.filename 
                     << "ID:" << file.id
                     << "大小:" << file.bytes;
        }
    });
    
    // 获取文件信息
    auto retrieveFuture = client.files()->retrieve("file-abc123");
    retrieveFuture.then([](FileResponse response) {
        auto file = response.file();
        qDebug() << "文件名:" << file.filename;
    });
    
    // 删除文件
    auto deleteFuture = client.files()->deleteFile("file-abc123");
    deleteFuture.then([](DeleteResponse response) {
        qDebug() << "删除成功:" << response.deleted();
    });
}
```

### 错误处理

```cpp
#include <QOpenAI>
#include <QDebug>

using namespace QOpenAI;

void errorHandlingExample()
{
    QOpenAI client;
    
    QList<ChatMessage> messages;
    ChatMessage msg;
    msg.role = MessageRole::User;
    msg.content = "你好";
    messages.append(msg);
    
    auto future = client.chat()->completions()->create("gpt-4o", messages);
    
    future.then([](ChatCompletionResponse response) {
        qDebug() << "成功:" << response.content();
        qDebug() << "请求 ID:" << response.requestId();
    }).onFailed([](const std::exception& e) {
        try {
            std::rethrow_exception(std::current_exception());
        } catch (const AuthenticationError& err) {
            qDebug() << "认证错误:" << err.message();
        } catch (const RateLimitError& err) {
            qDebug() << "速率限制:" << err.message();
        } catch (const APIConnectionError& err) {
            qDebug() << "连接错误:" << err.message();
        } catch (const APITimeoutError& err) {
            qDebug() << "超时错误:" << err.message();
        } catch (const APIStatusError& err) {
            qDebug() << "API 错误:" << err.statusCode() << err.message();
        } catch (const OpenAIError& err) {
            qDebug() << "OpenAI 错误:" << err.message();
        }
    });
}
```

### 高级配置

```cpp
#include <QOpenAI>
#include <QDebug>

using namespace QOpenAI;

void advancedConfigExample()
{
    // 自定义配置
    OpenAIConfig config;
    config.apiKey = "your-api-key";
    config.organization = "your-org-id";
    config.project = "your-project-id";
    
    // 自定义基础 URL
    config.baseUrl = QUrl("https://api.openai.com/v1");
    
    // 设置代理 - Clash 客户端（推荐）
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("127.0.0.1");  // Clash 默认本地地址
    proxy.setPort(7890);              // Clash 默认 HTTP 代理端口
    config.proxy = proxy;
    config.useProxy = true;
    
    // 或使用 Clash 的 SOCKS5 代理
    // QNetworkProxy socksProxy;
    // socksProxy.setType(QNetworkProxy::Socks5Proxy);
    // socksProxy.setHostName("127.0.0.1");
    // socksProxy.setPort(7891);  // Clash 默认 SOCKS5 端口
    // config.proxy = socksProxy;
    // config.useProxy = true;
    
    // 其他代理服务器
    // QNetworkProxy customProxy;
    // customProxy.setType(QNetworkProxy::HttpProxy);
    // customProxy.setHostName("proxy.example.com");
    // customProxy.setPort(8080);
    // customProxy.setUser("username");  // 如果需要认证
    // customProxy.setPassword("password");
    // config.proxy = customProxy;
    // config.useProxy = true;
    
    // 设置超时
    config.timeout = Timeout(120.0f); // 120 秒
    
    // 设置最大重试次数
    config.maxRetries = 3;
    
    // 添加自定义请求头
    config.defaultHeaders["X-Custom-Header"] = "custom-value";
    
    // 使用动态 API key
    config.apiKeyProvider = []() -> QString {
        // 从某处获取最新的 API key
        return getLatestApiKey();
    };
    
    QOpenAI client(config);
    
    // 每个请求的自定义选项
    RequestOptions options;
    options.timeout = Omit<Timeout>(Timeout(30.0f)); // 30 秒超时
    options.maxRetries = Omit<int>(1); // 只重试一次
    
    Headers extraHeaders;
    extraHeaders["X-Request-ID"] = "unique-request-id";
    options.extraHeaders = Omit<Headers>(extraHeaders);
    
    QList<ChatMessage> messages;
    auto future = client.chat()->completions()->create("gpt-4o", messages, options);
}

QString getLatestApiKey() {
    // 实现获取 API key 的逻辑
    return "sk-...";
}
```

### 使用环境变量

应用支持以下环境变量：

```bash
export OPENAI_API_KEY="your-api-key-here"
export OPENAI_ORG_ID="your-organization-id"
export OPENAI_PROJECT_ID="your-project-id"
export OPENAI_BASE_URL="https://api.openai.com/v1"
```

然后在代码中：

```cpp
// 自动从环境变量读取配置
QOpenAI client;
```

## API 参考

### 主要类

- `QOpenAI` - 主客户端类
- `QChat` - Chat API
- `ChatCompletions` - Chat Completions API
- `QCompletions` - Completions API (Legacy)
- `QEmbeddings` - Embeddings API
- `QModels` - Models API
- `QFiles` - Files API
- `QImages` - Images API
- `QModerations` - Moderations API

### 响应类型

- `ChatCompletionResponse` - Chat completion 响应
- `CompletionResponse` - Completion 响应
- `EmbeddingResponse` - Embedding 响应
- `ModelListResponse` - 模型列表响应
- `FileListResponse` - 文件列表响应
- `FileResponse` - 文件响应
- `ImageResponse` - 图像响应
- `DeleteResponse` - 删除响应

### 异常类型

- `OpenAIError` - 基础异常类
- `APIConnectionError` - 连接错误
- `APITimeoutError` - 超时错误
- `APIStatusError` - HTTP 状态错误
- `BadRequestError` - 400 错误
- `AuthenticationError` - 401 错误
- `PermissionDeniedError` - 403 错误
- `NotFoundError` - 404 错误
- `RateLimitError` - 429 错误
- `InternalServerError` - 500+ 错误

## 与 Python SDK 的对比

```python
# Python
from openai import OpenAI

client = OpenAI()
response = client.chat.completions.create(
    model="gpt-4o",
    messages=[
        {"role": "user", "content": "Hello!"}
    ]
)
print(response.choices[0].message.content)
```

```cpp
// C++ Qt
#include <QOpenAI>

using namespace QOpenAI;

QOpenAI client;
QList<ChatMessage> messages;
ChatMessage msg;
msg.role = MessageRole::User;
msg.content = "Hello!";
messages.append(msg);

auto future = client.chat()->completions()->create("gpt-4o", messages);
future.then([](ChatCompletionResponse response) {
    qDebug() << response.content();
});
```

## 待完成功能

- [ ] 流式响应支持 (SSE)
- [ ] 文件上传功能（多部分表单）
- [ ] 图像编辑和变体
- [ ] Fine-tuning API
- [ ] Assistants API
- [ ] Batch API
- [ ] Realtime API (WebSocket)
- [ ] Azure OpenAI 支持

## 许可证

MIT License

## 贡献

欢迎提交 Issue 和 Pull Request！
