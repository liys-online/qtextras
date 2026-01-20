# QOpenAI 快速入门指南

## 前提条件

1. Qt 5.15+ 或 Qt 6.x
2. CMake 3.16+
3. C++17 编译器
4. OpenAI API Key

## 安装步骤

### 1. 克隆或下载源码

```bash
cd qtextras/src/openai
```

### 2. 构建库

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 3. 安装（可选）

```bash
cmake --install .
```

## 配置 API Key

有三种方式配置 API Key：

### 方式 1: 环境变量（推荐）

```bash
# Windows
set OPENAI_API_KEY=sk-your-api-key-here

# Linux/macOS
export OPENAI_API_KEY=sk-your-api-key-here
```

### 方式 2: 代码中直接传入

```cpp
QOpenAI client("sk-your-api-key-here");
```

### 方式 3: 配置对象

```cpp
OpenAIConfig config;
config.apiKey = "sk-your-api-key-here";
QOpenAI client(config);
```

## 第一个程序

创建文件 `hello_openai.cpp`：

```cpp
#include <QOpenAI>
#include <QCoreApplication>
#include <QDebug>

using namespace QOpenAI;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    // 创建客户端
    QOpenAI client;
    
    // 准备消息
    QList<ChatMessage> messages;
    ChatMessage msg;
    msg.role = MessageRole::User;
    msg.content = "你好！";
    messages.append(msg);
    
    // 发送请求
    auto future = client.chat()->completions()->create("gpt-4o-mini", messages);
    
    // 处理响应
    future.then([&app](ChatCompletionResponse response) {
        qDebug() << response.content();
        app.quit();
    }).onFailed([&app](const std::exception& e) {
        qDebug() << "错误:" << e.what();
        app.exit(1);
    });
    
    return app.exec();
}
```

### 构建和运行

创建 `CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.16)
project(hello_openai)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_executable(hello_openai hello_openai.cpp)

target_link_libraries(hello_openai PRIVATE
    Qt6::Core
    Qt6::Network
    openai
)
```

构建：

```bash
mkdir build && cd build
cmake ..
cmake --build .
./hello_openai
```

## 常见用例

### 1. 对话

```cpp
QList<ChatMessage> messages;

// 系统消息
ChatMessage system;
system.role = MessageRole::System;
system.content = "你是一个编程助手";
messages.append(system);

// 用户消息
ChatMessage user;
user.role = MessageRole::User;
user.content = "如何使用 Qt 信号槽？";
messages.append(user);

auto future = client.chat()->completions()->create("gpt-4o", messages);
```

### 2. 文本嵌入

```cpp
auto future = client.embeddings()->create(
    "text-embedding-3-small",
    "需要向量化的文本"
);

future.then([](EmbeddingResponse response) {
    auto embeddings = response.embeddings();
    // 使用向量进行相似度计算等
});
```

### 3. 图像生成

```cpp
ImageGenerateParams params;
params.prompt = "一只可爱的小猫";
params.size = Omit<QString>("1024x1024");

auto future = client.images()->generate(params);

future.then([](ImageResponse response) {
    qDebug() << "图像 URL:" << response.urls().first();
});
```

### 4. 列出模型

```cpp
auto future = client.models()->list();

future.then([](ModelListResponse response) {
    for (const Model& model : response.models()) {
        qDebug() << model.id;
    }
});
```

## 错误处理

使用 try-catch 处理不同类型的错误：

```cpp
future.onFailed([](const std::exception& e) {
    try {
        std::rethrow_exception(std::current_exception());
    } catch (const AuthenticationError& err) {
        qDebug() << "API Key 错误";
    } catch (const RateLimitError& err) {
        qDebug() << "速率限制";
    } catch (const APITimeoutError& err) {
        qDebug() << "请求超时";
    } catch (const OpenAIError& err) {
        qDebug() << "OpenAI 错误:" << err.message();
    }
});
```

## 高级配置

```cpp
OpenAIConfig config;
config.apiKey = "your-api-key";
config.organization = "your-org";
config.timeout = Timeout(120.0f);  // 120 秒超时
config.maxRetries = 3;             // 最多重试 3 次

// 自定义请求头
config.defaultHeaders["X-Custom"] = "value";

QOpenAI client(config);
```

## 运行示例

项目包含多个示例程序：

```bash
cd build
./simple_chat          # 简单对话
./advanced_chat        # 高级功能
./embeddings_example   # 嵌入向量
./models_example       # 模型列表
```

## API 对比

### Python SDK

```python
from openai import OpenAI

client = OpenAI()
response = client.chat.completions.create(
    model="gpt-4o",
    messages=[{"role": "user", "content": "Hello!"}]
)
print(response.choices[0].message.content)
```

### QOpenAI (C++ Qt)

```cpp
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

用法基本一致，只是适配了 Qt 的异步模式（QFuture）。

## 获取帮助

- 查看 [README.md](README.md) 获取完整文档
- 查看 `examples/` 目录下的示例代码
- 参考 [OpenAI API 文档](https://platform.openai.com/docs/api-reference)

## 常见问题

**Q: 如何获取 API Key？**  
A: 访问 https://platform.openai.com/api-keys 创建

**Q: 支持流式响应吗？**  
A: 当前版本暂不支持，计划在后续版本添加

**Q: 如何设置代理？**  
A: 使用 Clash 客户端（推荐）：

```cpp
OpenAIConfig config;
config.apiKey = "your-api-key";

// Clash HTTP 代理（默认配置）
QNetworkProxy proxy;
proxy.setType(QNetworkProxy::HttpProxy);
proxy.setHostName("127.0.0.1");
proxy.setPort(7890);  // Clash 默认端口

config.proxy = proxy;
config.useProxy = true;

QOpenAI client(config);
```

或使用 SOCKS5（端口 7891）。支持 Clash、V2Ray 等客户端。详见 `examples/proxy_example.cpp`

**Q: Token 消耗如何计费？**  
A: 查看响应中的 `usage()` 字段了解消耗情况

## 下一步

- 探索更多 API 功能
- 集成到你的 Qt 应用中
- 实现自定义工具和插件
- 贡献代码改进项目
