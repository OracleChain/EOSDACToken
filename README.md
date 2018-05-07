前提：创建公私钥
doushadeMacBook-Pro:doc cl$ cleos create key
Private key: 5KYRmJzV2eRswvdQ6rvkeDMBWyh8Vq33FUY8PVmAZUbvAVeukzU
Public key: EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb
doushadeMacBook-Pro:doc cl$ cleos create key
Private key: 5Jqs5XFBqC787ywHjfUiT66HrEug5RCcqwgBsFYAX6nz8GU6W8t
Public key: EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et

cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ wallet create
PW5KhixDzG2zngXnkPDTZzPg98uNtPRubs9tSYGqFdb3Jw7DuskwE

cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ wallet import 5Jqs5XFBqC787ywHjfUiT66HrEug5RCcqwgBsFYAX6nz8GU6W8t
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ wallet import 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3


1.下载合约文件，
token合约地址
ssh://git@39.106.118.225:2200/lian/erc20currency.git

问答合约地址
ssh://git@39.106.118.225:2200/lian/oc_askanswer.git

2.下载后，编译合约
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ set contract erctoken ./ erc20currency.wast erc20currency.abi -p erctoken
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ set contract ocaskans ./ ocaskans.wast ocaskans.abi -p ocaskans
3.创建合约发布账户
	3.1 创建token管理账户
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ create account eosio erctoken EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et

	3.2创建问答管理账户
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ create account eosio ocaskans EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et


	3.3创建问答测试账户
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ create account eosio askera EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ create account eosio answera EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ create account eosio answerb EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ create account eosio answerc EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et


4.创建货币
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken create '[ "erctoken", "1000000000.0000 OCT", 0, 0, 0]' -p erctoken

5.发行货币
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken issue '[ "askera", "55.0000 OCT", "m" ]' -p erctoken
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken issue '[ "answera", "55.0000 OCT", "m" ]' -p erctoken
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken issue '[ "answerb", "55.0000 OCT", "m" ]' -p erctoken
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken issue '[ "answerc", "55.0000 OCT", "m" ]' -p erctoken


6.1转账
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/  push action erctoken transfer '[ "askera", "answera", "1.0000 OCT", "" ]' -p askera

6.2.授权可转走（押币）
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken approve '{"owner":"askera", "spender":"ocaskans", "quantity":"10.0000 OCT"}' -p askera

7.押币后提问
cleos  -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action ocaskans ask '{"id":"1", "from":"askera", "quantity":"2.0000 OCT","createtime":"0","endtime":"20000","optionanswerscnt":"3","asktitle":"what is you name","optionanswers":"{\"A\":\"成吉思汗\",\"B\":\"毛泽东\",\"C\":\"拿破仑\"}"}' -p askera


8.押币
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken approve '{"owner":"answerb", "spender":"ocaskans", "quantity":"10.0000 OCT"}' -p answerb
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken approve '{"owner":"answera", "spender":"ocaskans", "quantity":"10.0000 OCT"}' -p answera
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken approve '{"owner":"answerc", "spender":"ocaskans", "quantity":"10.0000 OCT"}' -p answerc

9.押币后回答问题
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/   push action ocaskans answer '{"from":"answera", "askid":"4","choosedanswer":"1"}' -p answera
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/   push action ocaskans answer '{"from":"answerb", "askid":"4","choosedanswer":"1"}' -p answerb
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/   push action ocaskans answer '{"from":"answerc", "askid":"4","choosedanswer":"2"}' -p answerc

10.释放问题，并自动奖励货币给回答者（包含回答问题时候押的币）
cleos  -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/  push action ocaskans releasemog '{"askid":"1"}' -p ocaskans


11.获取账户余额等信息
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/  get table erctoken erctoken accounts
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/  get table erctoken ocaskans accounts
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/  get table erctoken askera accounts
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/  get table erctoken answera accounts
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/  get table erctoken answerb accounts


12.获取问答列表信息
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ get table ocaskans ocaskans ask
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ get table ocaskans ocaskans answers 


辅助功能接口
>根据问题id删除问题
cleos  -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action ocaskans rmask '{"askid":"1"}' -p ocaskans
>从押币账户中转货币到自己账户上
cleos -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken transferfrom '{"from":"answerb", "to":"answera", "quantity":"1.0000 OCT"}' -p answera

cleos  -u http://127.0.0.1:8889/ --wallet-url http://127.0.0.1:8890/ push action erctoken balanceof '{"owner":"lucan111", "symbol":"OCT"}'

备注
错误码列表：
#define SUCCESS_CODE 0
#define PARAMETER_ERROR "6000001 PARAMETER_ERROR" //参数错误
#define BLANCE_NOT_ENOUGH "6000002 BLANCE_NOT_ENOUGH" //余额不足
#define ASK_NOT_EXISTS "6000006 ASK_NOT_EXISTS"//问题不存在
#define TIME_NOT_REACHED "6000007 TIME_NOT_REACHED"//时间未到
#define CANNOT_RELEASE_ASK_REPEAT "6000008 CANNOT_RELEASE_ASK_REPEAT"//问题已经释放，不能重复释放一个问题
#define BALANCE_SUPRE_UINT64_LIMIT "6000009 BALANCE_SUPRE_UINT64_LIMIT"//余额已经超过代码极限（理论永远不会发生，但除非恶搞极限测试）
#define ASK_RELEASED_CANNOT_ANSWER "6000010 ASK_RELEASED_CANNOT_ANSWER"//问题已经释放，不能huida
#define ILLEGAL_ANSWER "6000011 ILLEGAL_ANSWER"//非法回答，比如只有2个答案，结果用户选3或则-1
#define OPTIONS_ANSWERS_COUNT_SHOULE_BIGGER_THAN_ONE  "6000012 OPTIONS_ANSWERS_COUNT_SHOULE_BIGGER_THAN_ONE"
#define CANNOT_MODIFY_OTHERS_ASK  "6000013 CANNOT_MODIFY_OTHERS_ASK"
#define NOT_ENOUGH_OCT_TO_DO_IT  "6000014 NOT_ENOUGH_OCT_TO_DO_IT"//没有足够的oct
#define CANNOT_MODIFY_RELEASED_ASK  "6000015 CANNOT_MODIFY_RELEASED_ASK"//不能回答已经释放的问题
#define YOU_ANSWERED_THIS_QUESTION_EVER  "6000016 YOU_ANSWERED_THIS_QUESTION_EVER"//你已经曾经回答过这个问题
#define NOT_ENOUGH_ALLOWED_OCT_TO_DO_IT  "6000017 NOT_ENOUGH_ALLOWED_OCT_TO_DO_IT"//没有抵押足够的OCT
#define INVALID_QUANTITY "6000018 INVALID_QUANTITY"//无效额数
#define MUST_ISSUE_POSITIVE_QUANTITY "6000019 MUST_ISSUE_POSITIVE_QUANTITY"//额度必须为正整数
#define NSUFFICIENT_AUTHORITY "6000020 NSUFFICIENT_AUTHORITY"//没有足够授权
#define ACCOUNT_IS_FROZEN_BY_ISSUER "6000021 ACCOUNT_IS_FROZEN_BY_ISSUER"//账户倍发行者冻结
#define ALL_TRANSFERS_ARE_FROZEN_BY_ISSUER "6000022 ALL_TRANSFERS_ARE_FROZEN_BY_ISSUER"//不能给被冻结的账户转账
#define ACCOUNT_IS_NOT_WHITE_LISTED "6000023 ACCOUNT_IS_NOT_WHITE_LISTED"//账户没有在白名单
#define CAN_ONLY_TRANSFER_TO_WHITE_LISTED_ACCOUNTS "6000024 CAN_ONLY_TRANSFER_TO_WHITE_LISTED_ACCOUNTS"//只能转账给白名单账户
#define RECEIVER_REQUIRES_WHITELIST_BY_ISSUER "6000025 RECEIVER_REQUIRES_WHITELIST_BY_ISSUER"//货币状态或者接收方状态要求白名单属性
#define INVALID_SYMBOL_NAME "6000026 INVALID_SYMBOL_NAME"//无效的货币
#define TOKEN_WITH_SYMBOL_ALREADY_EXISTS "6000027 TOKEN_WITH_SYMBOL_ALREADY_EXISTS"//货币已经存在
