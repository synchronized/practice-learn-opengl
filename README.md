# practice-learn-opengl

基于 [LearnOpenGL 中文](https://learnopengl-cn.github.io/) 的练习工程，使用 CMake 构建，第三方库以子模块或源码直接置于 `3rd/` 目录（glfw、glad、glm、stb、assimp、imgui 等），无需 vcpkg。

## 依赖
- CMake ≥ 3.23
- C++17 编译器（MSVC/Clang/GCC）
- Git 子模块（如仓库未拉取，请先更新子模块）

```bash
git submodule update --init --recursive
```

## 构建
```bash
cmake -B build
cmake --build build
```

构建后示例可执行文件位于 `build/examples/<示例目录>/`。

## 示例目录
- `examples/01-hello-window`：窗口与渲染循环
- `examples/02-hello-trangle`：三角形/VAO/VBO
- `examples/03-shader`：着色器与 uniform
- `examples/04-texture`：纹理加载与采样
- `examples/05-transorm`：MVP 变换
- `examples/06-camera`：相机控制
- `examples/07-lighting`：基础光照
- `examples/08-material`：材质与高光
- `examples/09-light-casters`：点光/聚光/方向光
- `examples/10-model`：Assimp 模型加载（示例模型 `assets/models/FlightHelmet`）

公共组件位于 `examples/common`：
- `Shader`：加载/编译/链接着色器，设置 uniform
- `Texture`：使用 stb 加载纹理
- `Camera`：FPS 风格相机
- `Mesh` / `Model`：Assimp 模型解析与绘制（包含节点变换）

## 运行示例
以模型加载为例（构建后）：
- 可执行文件：`build/examples/10-model/10-model_main-01`
- 资源会复制到 `build/examples/10-model/assets`
- 鼠标控制视角，`WASDQE` 移动，`ESC` 退出

## 注意
- Windows/MSVC 已在 CMake 中启用 `/utf-8`。
- 需 OpenGL 3.3 Core Profile（GLFW hint 已设置）。
