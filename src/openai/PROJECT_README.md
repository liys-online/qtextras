# QOpenAI - Qt C++ OpenAI API 客户端库

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Qt](https://img.shields.io/badge/Qt-5.15%2B%20%7C%206.x-green.svg)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)

QOpenAI 是基于 [openai-python](https://github.com/openai/openai-python) SDK 实现的 Qt C++ 版本，提供类型安全、易用的 OpenAI API 访问接口。

## 项目结构

```
openai/
├── qopenai.h/cpp                 # 主客户端类
├── qopenaibaseclient.h/cpp       # 基础客户端（HTTP 请求处理）
├── qopenaichat.h/cpp             # Chat Completions API
├── qopenairesources.h/cpp        # 其他资源（Embeddings, Models 等）
├── qopenaitypes.h                # 类型定义
├── qopenaiexceptions.h           # 异常类
├── qopenairesponse.h/cpp         # 响应类
├── openai_global.h               # 导出宏
├── CMakeLists.txt                # 构建配置
├── README.md                     # 完整文档
├── QUICKSTART.md                 # 快速入门
├── examples/                     # 示例程序
│   ├── simple_chat.cpp          # 简单对话
│   ├── advanced_chat.cpp        # 高级功能
│   ├── embeddings_example.cpp   # 嵌入向量
│   └── models_example.cpp       # 模型列表
└── openai-python/                # 原始 Python SDK（参考）
```

## ✨ 特性

- ✅ **完整的 API 覆盖**
  - Chat Completions API
  - Completions API (Legacy)
  - Embeddings API
  - Models API
  - Files API
  - Images API (生成)
  - Moderations API

- ✅ **类型安全**
  - 强类型参数和响应
  - 编译时错误检查
  - IDE 自动完成支持

- ✅ **现代 C++**
  - C++17 标准
  - 智能指针
  - 异步操作 (QFuture)
  - 异常处理

- ✅ **Qt 集成**
  - QNetworkAccessManager
  - QFuture/QPromise
  - 信号槽支持
  - JSON 处理

- ✅ **健壮性**
  - 自动重试机制
  - 超时配置
  - 详细的错误类型
  - 请求 ID 追踪
  - 代理支持（HTTP/SOCKS5）

## 📦 安装

### 要求

- Qt 5.15+ 或 Qt 6.x
- CMake 3.16+
- C++17 兼容编译器

### 构建

```bash
git clone <repository-url>
cd qtextras/src/openai
mkdir build && cd build
cmake ..
cmake --build .
cmake --install .  # 可选
```

## 🚀 快速开始

### 设置 API Key

```bash
export OPENAI_API_KEY="sk-your-api-key-here"
```

### 第一个程序

```cpp
#include <QOpenAI>
#include <QCoreApplication>
#include <QDebug>

using namespace QOpenAI;

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    QOpenAI client;  // 自动从环境变量读取 API key
    
    QList<ChatMessage> messages;
    ChatMessage msg;
    msg.role = MessageRole::User;
    msg.content = "你好，介绍一下你自己";
    messages.append(msg);
    
    auto future = client.chat()->completions()->create("gpt-4o-mini", messages);
    
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

## 📖 使用示例

### Chat Completions

```cpp
// 使用系统消息
ChatCompletionCreateParams params;
params.model = "gpt-4o";

ChatMessage system;
system.role = MessageRole::System;
system.content = "你是一个专业的编程助手";
params.messages.append(system);

ChatMessage user;
user.role = MessageRole::User;
user.content = "如何使用智能指针？";
params.messages.append(user);

params.temperature = Omit<float>(0.7f);
params.maxTokens = Omit<int>(1000);

auto future = client.chat()->completions()->create(params);
```

### Embeddings

```cpp
auto future = client.embeddings()->create(
    "text-embedding-3-small",
    "需要向量化的文本"
);

future.then([](EmbeddingResponse response) {
    auto embeddings = response.embeddings();
    qDebug() << "向量维度:" << embeddings.first().size();
});
```

### 图像生成

```cpp
ImageGenerateParams params;
params.prompt = "一只可爱的小猫在弹钢琴";
params.model = Omit<QString>("dall-e-3");
params.size = Omit<QString>("1024x1024");

auto future = client.images()->generate(params);
```

### 错误处理

```cpp
future.onFailed([](const std::exception& e) {
    try {
        std::rethrow_exception(std::current_exception());
    } catch (const AuthenticationError& err) {
        qDebug() << "认证失败，请检查 API Key";
    } catch (const RateLimitError& err) {
        qDebug() << "达到速率限制";
    } catch (const APITimeoutError& err) {
        qDebug() << "请求超时";
    } catch (const OpenAIError& err) {
        qDebug() << "API 错误:" << err.message();
    }
});
```

## 📚 文档

- [快速入门](QUICKSTART.md) - 详细的入门教程
- [完整 API 文档](README.md) - API 参考和高级用法
- [示例代码](examples/) - 各种使用场景的示例

## 🔄 与 Python SDK 对比

| 功能 | Python | Qt C++ |
|------|--------|--------|
| Chat Completions | ✅ | ✅ |
| Streaming | ✅ | ⏳ |
| Embeddings | ✅ | ✅ |
| Images | ✅ | ✅ (生成) |
| Files | ✅ | ✅ (部分) |
| Models | ✅ | ✅ |
| Async | ✅ | ✅ (QFuture) |
| Type Safety | ⚠️ | ✅✅ |

## 🛣️ 路线图

- [x] 核心 API 实现
- [x] Chat Completions
- [x] Embeddings
- [x] Models
- [x] Images (生成)
- [x] 异常处理
- [x] 示例代码
- [ ] 流式响应 (SSE)
- [ ] 文件上传
- [ ] Fine-tuning API
- [ ] Assistants API
- [ ] Batch API
- [ ] Realtime API (WebSocket)
- [ ] Azure OpenAI 支持

## 🤝 贡献

欢迎贡献代码！请遵循以下步骤：

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情

## 🙏 致谢

- [OpenAI](https://openai.com/) - 提供强大的 API
- [openai-python](https://github.com/openai/openai-python) - Python SDK 参考
- [Qt](https://www.qt.io/) - 优秀的跨平台框架

## 📞 联系方式

如有问题或建议，请：
- 提交 Issue
- 发起 Discussion
- 提交 Pull Request

---

**注意**: 本库需要有效的 OpenAI API Key 才能使用。请访问 [OpenAI Platform](https://platform.openai.com/) 获取。
