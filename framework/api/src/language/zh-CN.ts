export default {
  keyword_input: '关键字',
  empty_text: '暂无数据',
  success: '成功',
  error: '失败',
  add: '添加',
  close: '关闭',
  delete: '删除',
  rename: '重命名',
  set: '设置',
  clean: '清空',
  help: '帮助',
  run: '运行',
  quit: '退出',
  operation_success: '操作成功',
  operation_failed: '操作失败',
  location_error: '当前 Location 错误',
  watcher_error: 'Watcher is NULL',
  instrument_error: '标的错误',
  component_error: '组件错误',
  board_empty: '未添加任何面板',
  add_board_now: '立即添加',
  prompt: '提示',
  warning: '警告',
  confirm: '确 认',
  cancel: '取 消',
  save_file: '保存文件',
  detail: '详情',
  ok: '好的',
  yes: '是',
  no: '否',

  edit: '编辑',
  copy: '复制',
  paste: '粘贴',
  select_all: '全选',
  undo: '撤销',

  open_resources_directory: '打开功夫资源目录 (KF_HOME)',
  open_install_directory: '打开功夫安装目录',
  open_basic_configuration: '打开功夫基础配置DB',
  browsing_log: '浏览日志文件',

  clear_journal: '清理journal',
  clear_DB: '清理DB',
  reset_main_panel: '重置主面板',
  export_all_transaction_data: '导出所有交易数据',

  website: '官网',
  user_manual: '用户手册',
  API_documentation: '策略API文档',
  Kungfu_forum: 'Kungfu 论坛',

  KungFu: '功夫',
  about_kungfu: '关于功夫交易',

  file: '文件',
  folder: '文件夹',

  no_focus: '当前页面没有聚焦',
  clear: '清理 {content} 完成, 请重启应用',
  open_window: '正在打开窗口',
  open_code_editor: '正在打开代码编辑器',
  open_trading_task_view: '正在打开交易任务视图',
  add_board: '添加面板',
  select_board: '请选择要添加的面板',
  add_board_error: '添加面板目标错误',
  select_date: '选择日期',
  date_type: '日期类型',
  natural_day: '自然日',
  trading_day: '交易日',

  delete_category:
    '删除 {category} 所有数据,  如果该 {categoryName} 进程正在运行, 也将停止进程, 确认删除',
  add_config_modal:
    '{category}ID系统唯一, {changeTypeName} 成功后不可修改, 确认 {key}',
  update_config_modal: '确认{key} 相关配置',
  database_locked: '检测到当前有交易进行, 数据库被占用',
  export_database_locked:
    '检测到当前有交易进行, 为不影响交易数据落地, 建议收盘后尝试进行此操作',

  MakeOrder: '下单面板',
  FutureArbitrage: '套利指令',
  BlockTrade: '大宗交易',
  OrderBook: '深度行情',
  MarketData: '行情订阅',
  TradingTask: '交易任务',
  Strategy: '策略进程',
  Md: '行情源',
  Td: '交易账户',
  Trade: '成交记录',
  Order: '委托记录',
  PosGlobal: '持仓汇总',
  Pos: '持仓',
  TransferRecord: '划转记录',

  please_wait: '请稍后',
  please_wait_and_retry: '请稍后重试',

  baseConfig: {
    main_panel: '主面板',
    control_center: '控制中心',
  },

  settingsFormConfig: {
    keyword: '关键词',
    add_csv: '导入 csv',
    csv_template: '下载 csv 模板',
    add_csv_desc: 'csv 表头为 {header}',
    clear: '清除',
    total: '共计 {sum} 条数据',
    import_successed: '导入成功',
    import_failed: '导入失败',
    csv_format_error: 'csv 格式错误, 请检查后重试',
  },

  tradingConfig: {
    unknown: '未知',
    default: '封装策略',
    order_task: '下单任务',

    running: '运行中',
    stopping: '停止中',
    stopped: '已停止',
    launching: '启动中',
    error: '错误',
    waiting_restart: '待重启',

    pending: '等待中',
    Idle: '无数据',
    dis_connected: '已断开',
    connected: '已连接',
    logged_in: '已登录',
    login_failed: '登录失败',
    ready: '就绪',

    system: '系统服务',
    daemon: '后台服务',
    md: '行情源',
    td: '交易账户 ',
    strategy: '策略',

    open: '开',
    close: '平',
    close_today: '平今',
    close_yesterday: '平昨',

    by_quantity: '按数量',
    by_proportion: '按比例',

    latest: '最新价',
    sell5: '卖五价',
    sell4: '卖四价',
    sell3: '卖三价',
    sell2: '卖二价',
    sell1: '卖一价',
    buy1: '买一价',
    buy2: '买二价',
    buy3: '买三价',
    buy4: '买四价',
    buy5: '买五价',
    up_limit_price: '涨停价',
    low_limit_price: '跌停价',

    buy: '买',
    sell: '卖',
    lock: '锁仓',
    unlock: '解锁',
    exec: '行权',
    drop: '放弃行权',
    purchase: '申购',
    redemption: '赎回',
    split: '拆分',
    merge: '合并',
    margin_trade: '融资买入',
    short_sell: '融券卖出',
    repay_margin: '卖券还款',
    repay_short: '买券还券',
    cash_repay_margin: '现金还款',
    stock_repay_short: '现券还券',
    surplus_stock_transfer: '余券划转',
    guarantee_stock_transfer: '担保品转入',
    guarantee_stock_redeem: '担保品转出',

    submitted: '已提交',
    cancelled: '已撤单',
    filled: '已成交',
    partial_filled_not_active: '部分撤单',
    partial_filled_active: '正在交易',
    lost: '丢失',

    long: '多',
    short: '空',

    Limit: '[Limit] 限价',
    Market: '[Any] 市价',
    FakBest5: '[FakBest5] 上海深圳最优五档即时成交剩余撤销, 不需要报价',
    Forward_best: '[ForwardBest] 深圳本方最优价格申报',
    Reverse_best:
      '[ReverseBest] 上海最优五档即时成交剩余转限价, 深圳对手方最优价格申报, 不需要报价',
    Fak: '[Fak] 深圳即时成交剩余撤销',
    Fok: '[Fok] 深圳市价全额成交或者撤销',

    speculation: '投机',
    hedge: '套保',
    arbitrage: '套利',
    covered: '备兑',

    any: '任意',
    min: '最小',
    all: '全部',

    by_amount: '按金额',
    by_volume: '按手数',

    IOC: 'IOC',
    GFD: 'GFD',
    GTC: 'GTC',

    stock: '股票',
    future: '期货',
    bond: '债券',
    stock_option: '股票期权',
    fund: '基金',
    tech_stock: '科技板股票',
    index: '指数',
    repo: '回购',
    warrant: '认权证',
    iopt: '牛熊证',
    crypto: '数字货币',
    crypto_future: '数字货币合约',
    multi: '多品种',

    SSE: '上交所',
    SZE: '深交所',
    BSE: '北交所',
    GFEX: '广交所',
    SHFE: '上期所',
    DCE: '大商所',
    CZCE: '郑商所',
    CFFEX: '中金所',
    INE: '能源中心',

    HK: '港股',
    HKFUT: '港期',
    US: '美股',
    USFUT: '美期',
    SGX: '新股',
    SGXFUT: '新期',
    EUR: '欧股',
    EURFUT: '欧期',
    LON: '英股',
    LONFUT: '英期',
    AEX: '荷股',
    AEXFUT: '荷期',
    AUX: '澳股',
    AUXFUT: '澳期',
    HEXS: '新股',
    HEXSFUT: '新期',
    IDX: '印尼股',
    IDXFUT: '印尼期',
    KORC: '韩碳所',
    LME: '伦金所',
    MYS: '马来股',
    MYSFUT: '马来期',
    ABB: '美布告',
    PRX: '法股',
    PRXFUT: '法期',
    SIX: '瑞股',
    SIXFUT: '瑞期',
    TAX: '泰股',
    TAXFUT: '泰期',
    JP: '日股',
    JPFUT: '日期',
    TSE: '多股',
    TSEFUT: '多期',
    XETRA: 'XETRA',
    GLFX: '外汇',
    IPE: 'IPE',
    CBOT: 'CBOT',
    CEC: 'CEC',
    LIFE: 'LIFE',
    MTIF: 'MTIF',
    NYCE: 'NYCE',
    CMX: 'CMX',
    NYME: 'NYME',
    SIME: 'SIME',
    CME: 'CME',
    IMM: 'IMM',
    WIDX: 'WIDX',
    FREX: 'FREX',
    METL: 'METL',
    IPM: '国际贵金属',

    SP: '大商所 跨期 SP',
    SPC: '大商所 跨品种 SPC',
    SPD: '郑商所 跨期 SPD',
    IPS: '郑商所 跨品种 IPS',

    CNY: 'CNY',
    HKD: 'HKD',
    USD: 'USD',
    JPY: 'JPY',
    GBP: 'GBP',
    EURO: 'EUR',
    CNH: 'CNH',
    SGD: 'SGD',
    MYR: 'MYR',

    master: '主控进程',
    ledger: '计算服务',
    cached: '存储服务',
    archive: '归档服务',

    place_order: '下单',
    apart_order: '拆单',
    reset_order: '重置',
    account: '账户',
    instrument: '标的',
    instrument_type: '标的类型',
    volume: '下单量',
    price: '价格',
    protect_price: '保护价格',
    price_type: '方式',
    price_level: '目标价格',
    price_offset: '偏移',
    side: '买卖',
    offset: '开平',
    direction: '多空',
    limit_price: '下单价格',
    algorithm: '算法',

    make_order_number: '下单次数',
    no_empty: '下单量不可为空',
    total_order_amount: '总下单量',
    every_volume: '每次下单量',

    fat_finger_buy_modal:
      '买入价格超出警戒线, 当前价格为 {price}, 警戒线为 {warningLine}, 当前乌龙指阈值为 {fatFinger}%',
    fat_finger_sell_modal:
      '卖出价格超出警戒线, 当前价格为 {price}, 警戒线为 {warningLine}, 当前乌龙指阈值为 {fatFinger}%',
    close_apart_open_modal:
      '下单量为 {volume}, 当前标的可平 {direction} 仓为 {closableVolume}, 超出数量为 {openVolume}\n点击 “超出部分反向开仓”, 将会 平 {direction} {closableVolume}, 开{oppositeDirection} {openVolume}\n点击“按原方案下单”, 将会继续平 {direction} {volume}',
    start_process: '请先启动{process}交易进程',
    place_confirm: '下单确认',
    continue_close_rate: '{relationship} 平仓阈值 ({rate}%), 是否继续下单?',
    Continue: '继续下单',
    original_plan: '按原方案下单',
    beyond_to_open: '超出部分反向开仓',
    reach: '达到',
    above: '超过',
  },

  orderConfig: {
    update_time: '更新时间',
    instrument_id: '标的',
    limit_price: '委托价',
    order_status: '订单状态',
    latency_system: '系统延迟μs',
    latency_network: '网络延迟μs',
    avg_price: '成交均价',
    dest_uname: '下单源',
    source_uname: '目标账户',
    completed: '已完成',
    clinch: '已成交',
    all: '全部',

    mean: '委托均价',
    max: '最高委托价',
    min: '最低委托价',
    volume: '成交量',

    checkbox_text: '未完成委托',
    cancel_all: '全部撤单',
    date_picker: '请选择日期',
    confirm_cancel_all: '确认全部撤单',
    entrust: '委托',
    start: '请先启动',
    td: '交易进程',
    confirm: '确认',

    entrust_statistical: '委托统计',
    statistical_desc: '实时',
    entrust_statistical_number: '委托统计数量',
    entrust_statistical_price: '委托价统计',
    average_withdrawal_ratio: '平均撤单比 (仅统计 部成部撤 和 全部撤单)',
    average_system_latency: '平均系统延迟(μs)',
    min_system_latency: '最小系统延迟(μs)',
    max_system_latency: '最大系统延迟(μs)',
    average_network_latency: '平均网络延迟(μs)',
    min_network_latency: '最小网络延迟(μs)',
    max_network_latency: '最大网络延迟(μs)',
  },

  tdConfig: {
    td_group: '账户分组',
    td_name: '账户组名称',
    account_name: '账户(组)',
    account_ps: '备注',
    state_status: '状态',
    process_status: '进程',
    unrealized_pnl: '浮动盈亏',
    marked_value: '市值',
    margin: '保证金',
    avail_money: '可用资金',
    avail_margin: '可用保证金',
    cash_debt: '融资负债',
    total_asset: '总资产',
    actions: '操作',

    add_td: '添加',
    add_group_placeholder: '添加分组',
    set_td_group: '账户分组设置',
    account_group: '账户组',
    td_not_found: '{td}柜台插件不存在',
    sourse_not_found: '配置项不存在, 请检查 {value} package.json',
    need_only_group: '需保证该账户组名称唯一',
    delete_amount_group: '删除账户组 {group}',
    confirm_delete_group: '不会影响改账户组下账户进程, 确认删除',
  },

  fund_trans: {
    config_error: '{td}柜台资金划转配置文件异常',
    modal_title: '柜台间资金划转',
    trans_selection: '请选择资金划转的方向',
    centralized_counter: '集中柜台',
    source: '划出节点',
    target: '划入节点',
    amount: '发生金额',
    update_time: '划转时间',
    tip_error: '划转失败,请联系管理员!',
    capitalaccountor: '资金账号',
    trade_password: '交易密码',
    account: '客户号',
    trans_type: '划转类型',
    pending: '等待中',
    status: '划转状态',
    success: '成功',
    error: '失败',
  },

  mdConfig: {
    counter_name: '柜台',
    state_status: '状态',
    process_status: '进程',
    actions: '操作',
    select_counter_api: '选择柜台API',
    select_trade_task: '选择交易任务',
    select_plugin_type: '选择插件类型',

    add_md: '添加',
    counter_plugin_inexistence: '柜台插件不存在',
  },

  strategyConfig: {
    strategy: '策略',
    strategy_id: '策略ID',
    strategy_file: '策略文件',
    strategy_path: '策略路径',
    strategy_path_tip:
      '普通 python 策略选择 .py 文件, 加密 python 策略或cpp策略选择编译后的 .so 或 .pyd 文件',
    process_status: '进程',
    unrealized_pnl: '浮动盈亏',
    marked_value: '市值',
    actions: '操作',
    strategy_tip: '需保证该策略ID唯一',

    add_strategy: '添加',
  },

  tradingTaskConfig: {
    task_id: '任务ID',
    process_status: '进程',
    args: '参数',
    actions: '操作',

    add_task: '添加',
    illegal_process_id: '不是合法交易任务进程ID',
    key_inexistence: '交易任务插件 key 不存在',
    plugin_inexistence: '交易任务插件不存在',
    configuration_inexistence: '配置项不存在, 请检查',
    delete_task: '删除交易任务',
    delete_task_content:
      '所有数据, 如果该交易任务正在运行, 也将停止进程, 确认删除',
  },

  posGlobalConfig: {
    instrument_id: '标的',
    account_id_resolved: '持有账户',
    yesterday_volume: '昨',
    today_volume: '今',
    sum_volume: '总',
    frozen_total: '冻结数量',
    frozen_volume: '冻结',
    closable_volume: '可平',
    avg_open_price: '开仓均价',
    last_price: '最新价',
    unrealized_pnl: '浮动盈亏',
  },

  marketDataConfig: {
    instrument_id: '标的',
    open_price: '开盘价',
    high_price: '最高价',
    low_price: '最低价',
    last_price: '最新价',
    volume: '成交量',
    actions: '操作',

    subscribe_btn: '订阅',
    add_market: '添加自选',
  },

  tradeConfig: {
    trade_time_resolved: '成交时间',
    kf_time_resolved: '系统时间',
    instrument_id: '标的',
    price: '成交价',
    volume: '成交量',
    latency_trade: '成交延迟(μs)',

    mean_price: '成交均价',
    min_price: '最低成交价',
    max_price: '最高成交价',

    statistical: '成交统计',
    statistical_count: '统计成交数量',
    statistical_price: '成交价统计',
    statistical_desc: '实时',
    average_trade_latency: '平均成交延迟(μs)',
    max_trade_latency: '最大成交延迟(μs)',
    min_trade_latency: '最小成交延迟(μs)',

    greater_than_limit_value: '当前标的下单{key}最大值为{value}',
  },

  futureArbitrageConfig: {
    account_name: '账户',
    future_arbitrage_code: '套利代码',
    instrument_buy_A: '买入标的 A',
    instrument_sell_A: '卖出标的 A',
    instrument_buy_B: '买入标的 B',
    instrument_sell_B: '卖出标的 B',
    side: '买卖',
    offset: '开平',
    hedge_flag: '套保',
    is_swap: '互换',
    price_type: '方式',
    limit_price: '价差 A-B',
    volume: '下单量',

    place_order: '下单',
    reset_order: '重置',
    only_corresponding: '只能对应',
  },

  blockTradeConfig: {
    account_name: '账户',
    instrument_id: '标的',
    side: '买卖',
    offset: '开平',
    hedge_flag: '套保',
    is_swap: '互换',
    price_type: '方式',
    limit_price: '价格',
    volume: '下单量',
    opponent_seat: '对方席位',
    match_number: '约定序号',
    is_specific: '减持类型',

    unrestricted_shares: '非受限股份',
    restricted_shares: '受限股份',

    place_order: '下单',
    reset_order: '重置',
    only_number: '该项只能为数字',
  },

  globalSettingConfig: {
    global_setting_title: '全局设置',
    system: '系统',
    log_level: '全局日志级别',
    for_all_log: '对系统内所有日志级别的设置',

    auto_restart_td: '交易进程自动重启',
    auto_restart_td_desc:
      '交易进程断开时是否自动重启, 如果打开, 则当交易进程出错后, 会尝试重连三次, 如果关闭, 则不会；在重启过程中（重启开始到交易进程就绪）, 策略内查询到的持仓会为0, 需要在策略内通过 on_deregister, on_broker_state_change这两个方法来判断柜台状态是否断开/重启就绪',

    language: '语言',
    select_language_desc: '选择语言, 修改后重启功夫生效',
    bypass_archive: '跳过归档',
    bypass_archive_desc:
      '仅删除上个交易日留下的journal与log文件, 不再压缩打包, 跳过归档后无法恢复之前的内存数据, 会加快启动速度',

    porformance: '性能',
    rocket_model: '开启极速模式',
    rocket_model_desc:
      '开启极速模式会极大的降低系统延迟 (只有当 CPU 核数大于 4 时才能开启) , 并会使 CPU 使用效率达到100%, 重启后生效',
    bypass_accounting: '跳过UI进程计算',
    bypass_accounting_desc:
      'UI进程不再处理计算逻辑, 完全通过计算进程更新数据, 减轻UI进程性能占用, 重启后生效',
    bypass_trading_data: '纯监控模式',
    bypass_trading_data_desc:
      '该模式下仅可监控进程运行状态, UI进行性能占用达到最低, 重启后生效',
    strategy: '策略',
    use_local_python: '使用本地Python',
    local_python_desc:
      '使用本地python启动策略, 需要 pip3 install {whl_dir_path} 内 *.whl 文件, 开启后需重启策略, 本地 python3 版本需为 {py_version}',
    python_path: '选择本地 Python 路径',
    python_path_desc:
      '功夫将会以选择的python路径运行策略, 同时需要保证 kungfu*.whl 已经通过 pip安装',

    currency: '币种',
    instrument_currency: '标的币种',
    instrument_currency_desc: '打开时，会在持仓面板的标的列展示标的币种',

    trade: '交易',
    sound: '成交提示音',
    use_sound: '启用成交提示音',
    fat_finger_threshold: '乌龙指阈值',
    set_fat_finger: '设置乌龙指触发阈值(百分比)',
    close_threshold: '平仓阈值',
    set_close_threshold: '设置平仓阈值(百分比); 设置为 0% 时, 则视作关闭此项',
    trade_limit: '交易限制',
    set_trade_limit: '设置交易限制',
    order_input_key: '限制属性',
    single_price: '单笔成交价',
    limit_value: '最大值',
    asset_margin: '两融',
    show_asset_margin: '展示两融',

    code_editor: '代码编辑器',
    tab_space_type: '缩进类别',
    set_tab_space: '功夫编辑器缩进类别',
    tab_space_size: '缩进长度',
    set_tab_space_size: '功夫编辑器缩进长度(空格)',

    comission: '期货手续费',
    varieties: '品种',
    add_comission: '添加',
    save_comission: '保存',
    exchange_id: '交易所',
    open: '开仓',
    close_today: '平今',
    close_yesterday: '平昨',
    min: '最小',

    update: '版本更新',
    is_check_version: '检测更新',
    is_check_version_desc: '启动功夫时, 是否检测更新',
    current_version: '当前版本',
    already_latest_version: '已是最新版本',
    new_version: '新版本',
    start_download: '开始下载',
    find_new_version: '发现新版本: {version}\n是否现在下载安装包? ',
    downloaded: '下载完成, 等待安装',
    to_install: '现在安装',
    warning_before_install:
      '安装会清理当日交易数据并退出功夫 (如有需要请提前备份), 确定现在安装吗? (建议盘后进行)',
  },

  风控: '风控',
  风控描述: '启用风险控制, 当持仓量超过阈值时自动平仓',
  账户: '账户',
  柜台: '柜台',
  单比最大量: '单比最大量',
  每日最大成交量: '每日最大成交量',
  防止自成交: '防止自成交',
  最大回撤率: '最大回撤率',
  标的白名单: '标的白名单',
  白名单设置警告: '请先为此账户设置标的白名单',

  validate: {
    no_special_characters: '不能含有特殊字符和中文',
    single_characters: '必须包含数字和字母',
    no_underline: '不能含有下划线',
    no_zero_number: '请输入非零数字',
    no_negative_number: '请输入非负数',
    value_existing: '{value}已存在',
    mandatory: '该项为必填项',
    resolved_tip: '成功匹配 {success} 个{value}, 失败 {fail} 个',
  },

  editor: {
    set_strategy_entrance: '设置策略入口文件',
    current_strategy: '当前策略',
    new_file: '新建文件',
    new_folder: '新建文件夹',
    set_strategy_success: '策略 {file} 文件路径修改成功！',
    entry_file: '入口文件',
    creaate_success: '{file} 创建成功！',

    name_repeat: '此位置已存在文件或文件夹 {name}, 请选择其他名称！',
    empty_input: '必须提供文件或文件夹名称！',
    illegal_character: '名称不能包含\\/:*?"<>|',
    delate_confirm: '确认删除 {value} 吗？',
    cannot_delate_entry: '不可删除入口文件',
  },

  logview: {
    scroll_to_bottom: '滚动到底部',
  },

  master_interrupt: '主控进程 master 中断',
  master_desc: '主控进程负责策略进程间通信与资源配置, 请重启功夫交易系统',

  ledger_interrupt: '计算服务 ledger 中断',
  ledger_desc: '计算服务负责持仓跟资金计算, 请重启功夫交易系统',

  cached_interrupt: '存储服务 cached 中断',
  cached_desc:
    '存储服务负责数据落地, 存储服务断开不影响交易, 可等交易完成后重启功夫系统',

  state_interrupt_msg: '{state} 已断开',
  state_interrupt_desc: '{state} 已断开, 可能会导致交易中断, 请检查',

  kungfu: '功夫交易系统',

  system_prompt: '系统提示',
  computer_performance_done: '性能检测已完成 ✓',
  computer_performance_detecting: '性能检测中...',
  archive_done: '功夫归档完成 ✓',
  archive_loading: '功夫归档中...',
  environment_done: '功夫环境准备完成 ✓',
  environment_loading: '功夫环境准备中...',
  extra_resources_done: '插件资源加载就绪 ✓',
  extra_resouces_loading: '插件资源加载中...',
  saving_data_done: '保存数据完成 ✓',
  saving_data_loading: '保存数据中...',
  end_all_transactions: '结束所有交易进程 ✓',
  closing: '结束交易进程中, 请勿关闭...',

  computer_performance_abnormal:
    '电脑性能过低, 将默认开启跳过 UI 计算选项 (可在全局系统设置里打开, 开启可能会导致系统进程崩溃), 建议使用8核及以上的 CPU 运行系统',

  quit_confirm: '退出应用会结束所有交易进程, 确认退出吗?',
  restart_process: '功夫图形进程中断, 该中断不会影响交易, 是否重启图形进程？',

  未就绪: '{processId} 还未准备就绪, 请稍后重试',
  系统外: '系统外',
  手动: '手动',
  任务: '任务',

  正常: '正常',
  异常: '异常',
  错误: '错误',
  文件路径不存在: '文件路径不存在',
  策略id不存在: '策略id不存在',

  可用仓位: '可用仓位',
  可用资金: '可用资金',
  交易金额: '交易金额',
  保证金占用: '保证金占用',
  保证金返还: '保证金返还',
  剩余资金: '剩余资金',
  剩余仓位: '剩余仓位',
};
