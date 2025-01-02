`conda` 是一个非常流行的包管理器和环境管理工具，主要用于 Python 和数据科学生态系统。以下是一些常规的用法和场景：

------

## 1. **环境管理**

### 创建新环境

```bash
conda create -n my_env python=3.9
```

- 创建一个名为 `my_env` 的环境，指定 Python 版本为 3.9。

### 激活环境

```bash
conda activate my_env
```

- 切换到 `my_env` 环境。

### 退出环境

```bash
conda deactivate
```

- 退出当前激活的环境。

### 删除环境

```bash
conda remove -n my_env --all
```

- 删除名为 `my_env` 的环境及其所有包。

------

## 2. **包管理**

### 安装包

```bash
conda install numpy
```

- 安装 `numpy` 包到当前激活的环境中。

### 安装特定版本的包

```bash
conda install numpy=1.21
```

- 安装指定版本的 `numpy`。

### 更新包

```bash
conda update numpy
```

- 更新 `numpy` 包到最新版本。

### 卸载包

```bash
conda remove numpy
```

- 从当前环境中卸载 `numpy`。

------

## 3. **环境导出与共享**

### 导出环境

```bash
conda env export > environment.yml
```

- 将当前环境保存为 `environment.yml` 文件，便于共享。

### 从文件创建环境

```bash
conda env create -f environment.yml
```

- 从 `environment.yml` 文件中重建环境。

------

## 4. **查看信息**

### 查看所有环境

```bash
conda env list
```

或

```bash
conda info --envs
```

- 列出所有已创建的环境。

### 查看已安装的包

```bash
conda list
```

- 列出当前环境中安装的所有包。

------

## 5. **清理空间**

### 删除未使用的包和缓存

```bash
conda clean --all
```

- 清理无用的包、缓存和索引。

------

## 6. **切换源（镜像加速）**

默认情况下，`conda` 使用国外源，速度可能较慢。可以切换到国内镜像源（例如清华大学）。

### 配置清华镜像

```bash
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/
conda config --set show_channel_urls yes
```

------

## 7. **使用虚拟环境**

在创建和管理环境时，`conda` 提供了比 `virtualenv` 更强大的功能，尤其是与非 Python 包的兼容性，例如 C 库。

### 与 pip 一起使用

```bash
conda install pip
pip install package_name
```

- 在 Conda 环境中使用 `pip` 安装特定的包。

------

## 8. **查看帮助**

### Conda 帮助命令

```bash
conda --help
conda create --help
conda install --help
```

- 查看 Conda 和子命令的帮助信息。

------

## 9. **升级 Conda**

### 升级 Conda 自身

```bash
conda update conda
```

------

## 常见问题与解决

1. **如果 Conda 的某些包安装较慢或失败**：

   - 切换到国内镜像源。

   - 使用 

     ```
     conda-forge
     ```

      通道：

     ```bash
     conda config --add channels conda-forge
     ```

2. **环境冲突问题**：

   - 如果包之间存在依赖冲突，可以尝试指定安装顺序或版本。

3. **解决 Conda 和 pip 冲突**：

   - 优先使用 Conda 安装包，只有 Conda 中没有的包才使用 pip。

------

如果你有具体的需求或问题，我可以进一步帮助！