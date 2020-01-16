export const getSystemConfig = (pythonVersion: string) => ({
    "performance": {
        "key": "performance",
        "name": "性能",
        "cli": true,
        "config": [
            {
                "key": "rocket",
                "name": "开启极速模式",
                "cli": true,
                "tip": "开启极速模式会使 CPU 达到100%，开启后请重启 Kungfu",
                "default": false,
                "type": "bool"
            }
        ]
    },
    "strategy": {
        "key": "strategy",
        "name": "策略",
        "cli": true,
        "config": [
            {
                "key": "python",
                "name": "使用本地python",
                "cli": true,
                "tip": `使用本地python启动策略，需要 pip3 install kungfu*.whl，本地 python3 版本需为 ${pythonVersion}，开启后需重启策略`,
                "default": false,
                "type": "bool"
            }
        ]
    },

    "log": {
        "key": "log",
        "name": "日志",
        "cli": true,
        "config": [
            {
                "key": "level",
                "name": "级别",
                "tip": "对系统内所有日志级别的设置",
                "type": "select",
                "data": [
                    { "value": "-l trace", "name": "TRACE" },
                    { "value": "-l debug", "name": "DEBUG" },
                    { "value": "-l info", "name": "INFO" },
                    { "value": "-l warning", "name": "WARN" },
                    { "value":"-l error", "name": "ERROR" },
                    { "value":"-l critical", "name": "CRITICAL" }
                ]
            }
        ]
    },

    "code": {
        "key": "code",
        "name": "编辑器",
        "cli": false,
        "config": [
            {
                "key": "tabSpaceType",
                "name": "缩进类别",
                "tip": "kungfu 编辑器缩进类别",
                "type": "select",
                "data": [
                    {"value": "Spaces", "name": "Spaces"},
                    {"value": "Tabs", "name": "Tabs"}
                ]
            },
            {
                "key": "tabSpaceSize",
                "name": "缩进大小",
                "tip": "kungfu 编辑器缩进大小（空格）",
                "type": "select",
                "data": [
                    {"value": 2, "name": 2},
                    {"value": 4, "name": 4}
                ]
            }
        ]
    }
})
