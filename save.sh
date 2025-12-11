#!/bin/bash
git add .
git commit -m "$1"

# 检查提交是否成功
if [ $? -eq 0 ]; then
    echo "提交成功，开始推送所有分支..."
    # 推送所有分支到 github 远程仓库
    git push github --all
    if [ $? -eq 0 ]; then
        echo "推送所有分支完成: $1"
    else
        echo "推送所有分支失败，请检查网络或冲突: $1"
    fi
else
    echo "提交失败: $1"
fi
