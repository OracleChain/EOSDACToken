
1.下载合约文件，
token合约地址
https://github.com/OracleChain/eosErc20CurrencyContract.git
问答合约地址
https://github.com/OracleChain/oc_askanswer.git

2.下载后，编译合约

3.创建合约发布账户
	3.1 创建token管理账户
cleos create account eosio erctoken EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et

	3.2创建问答管理账户
cleos create account eosio ocaskans EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et


	3.3创建问答测试账户
cleos create account eosio askera EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et
cleos create account eosio answera EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et
cleos create account eosio answerb EOS8gb6EkQDry72Ugwwy6MUZCr7EQ4ytGAeRhu3knVsA3qSneWPEb EOS6BB3rfssiBdiBN2d14GQLswcM1KBmcKWEM8fiSmdLw6eKWP5et


4.创建货币
cleos push action erctoken create '[ "erctoken", "1000000000.0000 OCT", 0, 0, 0]' -p erctoken

5.发行货币
cleos push action erctoken issue '[ "askera", "5.0000 OCT", "m" ]' -p erctoken
cleos push action erctoken issue '[ "answera", "5.0000 OCT", "m" ]' -p erctoken
cleos push action erctoken issue '[ "answerb", "5.0000 OCT", "m" ]' -p erctoken


6.授权可转账（押币）
cleos push action erctoken approve '{"owner":"askera", "spender":"ocaskans", "quantity":"2.0000 OCT"}' -p askera

7.押币后提问
cleos  push action ocaskans ask '{"id":"1", "from":"askera", "quantity":"2.0000 OCT","createtime":"0","endtime":"200","optionanswerscnt":"3","asktitle":"what is you name","optionanswers":"{\"A\":\"成吉思汗\",\"B\":\"毛泽东\",\"C\":\"拿破仑\"}"}' -p askera


8.押币
cleos push action erctoken approve '{"owner":"answera", "spender":"ocaskans", "quantity":"1.0000 OCT"}' -p answera

9.押币后回答问题
cleos   push action ocaskans answer '{"from":"answera", "askid":"2","choosedanswer":"1"}' -p answera

10.释放问题，并自动奖励货币给回答者（包含回答问题时候押的币）
cleos   push action ocaskans releasemog '{"askid":"1"}' -p ocaskans


11.获取账户余额等信息
cleos get table erctoken erctoken accounts
cleos get table erctoken ocaskans accounts
cleos get table erctoken askera accounts
cleos get table erctoken answera accounts
cleos get table erctoken answerb accounts


12.获取问答列表信息
cleos get table ocaskans ocaskans ask
cleos get table ocaskans ocaskans answers 


辅助功能接口
>根据问题id删除问题
cleos   push action ocaskans rmask '{"askid":"1"}' -p ocaskans
>从押币账户中转货币到自己账户上
cleos push action erctoken transferfrom '{"from":"answerb", "to":"answera", "quantity":"1.0000 OCT"}' -p answera

