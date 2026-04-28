## 必备插件

| 插件 | 来源 |
|------|------|
| **C/C++** | Microsoft |
| **CMake Tools** | Microsoft |

  

---

  

## 四步上手

  

```

① Ctrl+Shift+P → CMake: Select a Kit   → 选编译器

② 点击底部 "CMake: [Debug]: Ready"     → Configure（生成 build）

③ F7                                      → Build

④ 点击 ▶ 或 Ctrl+F5                       → Run / Debug

```

  

---

  

## CMakeLists.txt 最佳实践

  

**推荐写法：**

```cmake

add_library(lib_linklist STATIC LinkList/LinkList.cpp)

target_include_directories(lib_linklist PUBLIC LinkList)

```

  

避免使用全局 `include_directories()`，改用 `target_include_directories()` 让每个库管理自己的头文件路径。

  

---

  

## 常见问题

  

| 问题 | 解决 |

|------|------|

| 找不到头文件 | 底部栏选对 Configuration，或改 `c_cpp_properties.json` |

| 找不到编译器 | 确认 MinGW/GCC/Clang 已加入 `PATH` |

| `No kit selected` | 先执行 Select a Kit |

| build 混乱 | `rm -rf build`，重新 Configure |

  

---


## 快捷键


| 操作 | 快捷键 |
|------|--------|
| 构建 | `F7` |
| 运行 | `Ctrl+Shift+P` → Run Without Debugging |
| 调试 | `Ctrl+F5` |
| 选择 Kit | `Ctrl+Shift+P` → CMake: Select a Kit |

---

> **编译产物位置：** `build/` 目录（`.exe`、`.a` 库文件都在这）