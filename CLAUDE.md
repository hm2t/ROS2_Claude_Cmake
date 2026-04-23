# Karpathy Guidelines

Behavioral guidelines to reduce common LLM coding mistakes. **Always apply these guidelines.**

**Tradeoff:** Bias toward caution over speed. For trivial tasks, use judgment.

## 1. Think Before Coding

- State your assumptions explicitly. If uncertain, ask.
- If multiple interpretations exist, present them - don't pick silently.
- If a simpler approach exists, say so. Push back when warranted.
- If something is unclear, stop. Name what's confusing. Ask.

## 2. Simplicity First

- No features beyond what was asked.
- No abstractions for single-use code.
- No "flexibility" that wasn't requested.
- No error handling for impossible scenarios.
- If you write 200 lines and it could be 50, rewrite it.

## 3. Surgical Changes

- Don't "improve" adjacent code, comments, or formatting.
- Don't refactor things that aren't broken.
- Match existing style, even if you'd do it differently.
- If you notice unrelated dead code, mention it - don't delete it.
- The test: Every changed line should trace directly to the user's request.

## 4. Goal-Driven Execution

Transform tasks into verifiable goals:
- "Add validation" → "Write tests for invalid inputs, then make them pass"
- "Fix the bug" → "Write a test that reproduces it, then make it pass"
- "Refactor X" → "Ensure tests pass before and after"

For multi-step tasks, state a brief plan then verify each step.

---

# ROS2 Cmake 项目 (ament_cmake)



## ROS2 节点运行

- **运行节点:** `ros2 run <package> <node_name>`
- **构建包:** `colcon build --packages-select <package>`
- **创建 Python 包:** `ros2 pkg create --build-type ament_python --destination-directory src <pkg_name> --dependencies rclpy [deps]`
- **创建 C++ 包:** `ros2 pkg create --build-type ament_cmake --destination-directory src <pkg_name> --dependencies rclcpp std_msgs`
- **CMake 可执行文件:** `add_executable(<node> src/<node>.cpp)` + `ament_target_dependencies(<node> rclcpp std_msgs)`
- **安装节点:** `install(TARGETS <node> DESTINATION lib/${PROJECT_NAME})`
- **使用内置消息:** `std_msgs::msg::String` 用于发布订阅，无需自定义msg

## 构建与测试

- **Source 工作区:** `source install/setup.bash`
- **构建所有包:** `colcon build`
- **构建单个包:** `colcon build --packages-select <package>`
- **运行测试:** `colcon test` 或 `colcon test --packages-select <package>`
- **运行 lint 测试:** `colcon test --packages-select <package> --event-handlers console_direct+`

## ROS2 自定义接口 (ament_cmake)

- **CMakeLists.txt:** 使用 `rosidl_generate_interfaces(${PROJECT_NAME} "msg/..." "srv/..." "action/...")`
- **package.xml:** 添加 `<member_of_group>rosidl_interface_packages</member_of_group>`
- **依赖:** `rosidl_default_generators` + `rosidl_default_runtime`
- **接口目录:** `msg/`、`srv/`、`action/`

## ROS2 包创建规范

- **创建 ROS2 包:** 必须使用 `ros2 pkg create`，禁止手动 mkdir 创建目录结构

## Git 规范

提交格式：`type: 简短描述(用中文描述)`
type: feat | fix | docs | test | refactor | chore

示例：`feat: 添加用户登录功能`

## Git 分支规范

- **main** - 主分支，稳定版本
- **develop** - 开发分支，所有新功能在此开发
- 新功能开发 → `develop` → 合并到 `main`

## ⚠️ PR 强制规范

**所有合并必须通过 PR，禁止直接合并！**

| 合并方向 | 是否需要 PR |
|----------|------------|
| feature/fix → develop | ✅ 必须 PR |
| develop → main | ✅ 必须 PR |
| 其他分支合并 | ✅ 必须 PR |

**如果发现直接合并而非 PR，必须警告用户！**

**⚠️ Git PR 合并安全规则**

- `gh pr merge --squash --delete-branch` 会删除源分支，合并前必须确认！
- `develop → main` 合并时，禁止使用 `--delete-branch`（会删除 develop）
- feature/fix → develop 合并时，**必须先询问用户**是否删除源分支，再执行合并
- 如果用户未明确要求删除分支，默认使用 `--no-delete-branch`

## 项目结构

```
src/
├── cmake_pubsub/       # 发布订阅功能包
│   ├── src/            # C++ 源文件
│   ├── include/        # 头文件
│   ├── CMakeLists.txt  # 构建配置
│   └── package.xml     # 包依赖
├── build/              # colcon build 输出
├── install/            # source install/setup.bash
└── log/                # 日志
```

## Git 工作流

### 创建 feature 分支
```bash
git checkout develop && git pull && git checkout -b feature/your-feature
```

### PR 合并（必须确认删除选项）
- `develop → main`：`gh pr merge <pr> --squash`（禁止 --delete-branch）
- `feature → develop`：`gh pr merge <pr> --squash`（先询问是否删除 feature）

## ROS2 工作区

- **Source 工作区:** `source install/setup.bash` (构建后必须执行)
- **查看包列表:** `ros2 pkg list`
- **验证发布订阅:** `ros2 topic list` / `ros2 topic echo /topic` / `ros2 node list`