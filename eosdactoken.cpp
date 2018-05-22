/**
 * author chenlian@oraclechain.io
 * version 1.0
 * source reference eosdactoken contract,thanks eos contracts/eosdactoken contract
 * this eosdactoken follow the erc2.0 norms(trans,issue,approve,transferfrom,balanceof,allowance)
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include "eosdactoken.hpp"
#include"../oc_askanswer/tool.hpp"

EOSIO_ABI( eosdactoken, (transfer)(create)(issue)(transferfee)(approve)(transferfrom)(balanceof)(allowance))

using std::string;
using std::array;
using namespace eosio;


    void eosdactoken::issue(account_name to,
                         asset        quantity,
                         string       memo) {


        eosio_assert( quantity.symbol.is_valid(), INVALID_SYMBOL_NAME);

        auto sym = quantity.symbol.name();
        Stats statstable( _self, sym );
        const auto& ite = statstable.find( sym );
        const auto& st = *ite;

        eosio_assert( ite != statstable.end(),  TOKEN_WITH_SYMBOL_DOES_NOT_EXIST_CREATE_TOKEN_BEFORE_ISSUE);

        require_auth( st.issuer);

        eosio_assert( quantity.is_valid(), INVALID_QUANTITY );
        eosio_assert( quantity.amount > 0, MUST_ISSUE_POSITIVE_QUANTITY );

        eosio_assert( quantity.symbol == st.supply.symbol, SYMBOL_PRECISION_MISMATCH);
        eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, QUANTITY_EXCEEDS_AVAILABLE_SUPPLY);

        statstable.modify( st, 0, [&]( auto& s ) {
           s.supply.amount += quantity.amount;
        });

        add_balance( st.issuer, quantity, st, st.issuer );

        if( to != st.issuer )
        {
           SEND_INLINE_ACTION( *this, transfer, {st.issuer,N(active)}, {st.issuer, to, quantity, memo} );
        }
    }

    void eosdactoken::transferfee(
            account_name from,
            account_name to,
            asset        quantity,
            account_name tofeeadmin,
            asset        feequantity,
            string       memo){

          require_auth(from);

          eosio_assert( from != to, CANNOT_TRANSFER_TO_YOURSELF);
          eosio_assert( from != tofeeadmin, CANNOT_TRANSFER_TO_YOURSELF);

          eosio_assert( is_account( to ), TO_ACCOUNT_DOES_NOT_EXIST);
          eosio_assert( is_account( tofeeadmin ), TO_ACCOUNT_DOES_NOT_EXIST);

          auto sym = quantity.symbol.name();
          Stats statstable( _self, sym );
          const auto& st = statstable.get( sym );

          require_recipient( from );
          require_recipient( to );
          require_recipient( tofeeadmin );


          eosio_assert( quantity.is_valid(), INVALID_QUANTITY );
          eosio_assert( quantity.amount > 0, MUST_ISSUE_POSITIVE_QUANTITY );
          eosio_assert( quantity.symbol == st.supply.symbol,  SYMBOL_PRECISION_MISMATCH);


          eosio_assert( feequantity.is_valid(), INVALID_QUANTITY );
          eosio_assert( feequantity.amount > 0, MUST_ISSUE_POSITIVE_QUANTITY );
          eosio_assert( feequantity.symbol == st.supply.symbol,  SYMBOL_PRECISION_MISMATCH);

          sub_balance( from, quantity, st );
          add_balance( to, quantity, st, from );

          sub_balance( from, feequantity, st );
          add_balance( tofeeadmin, feequantity, st, from );
    }

    void eosdactoken::transfer(  account_name from,
                              account_name to,
                              asset        quantity,
                              string       memo) {
        require_auth(from);

        eosio_assert( from != to, CANNOT_TRANSFER_TO_YOURSELF);
        eosio_assert( is_account( to ), TO_ACCOUNT_DOES_NOT_EXIST);

        auto sym = quantity.symbol.name();
        Stats statstable( _self, sym );
        const auto& st = statstable.get( sym );

        require_recipient( from );
        require_recipient( to );

        eosio_assert( quantity.is_valid(), INVALID_QUANTITY );
        eosio_assert( quantity.amount > 0, MUST_ISSUE_POSITIVE_QUANTITY );
        eosio_assert( quantity.symbol == st.supply.symbol,  SYMBOL_PRECISION_MISMATCH);

        sub_balance( from, quantity, st );
        add_balance( to, quantity, st, from );
    }

    void eosdactoken::sub_balancefrom( account_name owner, asset value, const curstats& st ) {
       Accounts from_acnts( _self, owner );

       const auto& from = from_acnts.get( value.symbol.name() );
       eosio_assert( from.balance.amount >= value.amount, BLANCE_NOT_ENOUGH );


       if( from.balance.amount == value.amount ) {
            from_acnts.erase( from );
         } else {
            from_acnts.modify( from, 0, [&]( auto& a ) {
                a.balance -= value;
            });
       }
    }

    void eosdactoken::add_balancefrom( account_name owner, asset value, const curstats& st)
    {
       require_auth( owner );

       Accounts to_acnts( _self, owner );
       auto to = to_acnts.find( value.symbol.name() );
       if( to == to_acnts.end() ) {
          to_acnts.emplace( owner, [&]( auto& a ){
             a.balance = value;
          });
       } else {
          to_acnts.modify( to, owner, [&]( auto& a ) {
            a.balance += value;
          });
       }
    }

    void eosdactoken::sub_balance( account_name owner, asset value, const curstats& st ) {
       Accounts from_acnts( _self, owner );

       const auto& from = from_acnts.get( value.symbol.name() );
       eosio_assert( from.balance.amount >= value.amount, BLANCE_NOT_ENOUGH );

       //require_auth( owner );

       if( from.balance.amount == value.amount ) {
            from_acnts.erase( from );
         } else {
            from_acnts.modify( from, owner, [&]( auto& a ) {
                a.balance -= value;
            });
       }
    }

    void eosdactoken::add_balance( account_name owner, asset value, const curstats& st, account_name ram_payer )
    {
       Accounts to_acnts( _self, owner );
       auto to = to_acnts.find( value.symbol.name() );
       if( to == to_acnts.end() ) {
          to_acnts.emplace( ram_payer, [&]( auto& a ){
             a.balance = value;
          });
       } else {
          to_acnts.modify( to, 0, [&]( auto& a ) {
            a.balance += value;
          });
       }
    }


    void eosdactoken::approve(account_name owner,
                           account_name spender,
                           asset quantity){

        require_auth(owner);

        eosio_assert( is_account( spender ), TO_ACCOUNT_DOES_NOT_EXIST);

        Accounts from_acnts( _self, owner );
        const auto& from = from_acnts.find( quantity.symbol.name());
        eosio_assert(quantity.amount>0, APPROVE_QUANTITY_MUST_POSITIVE);
        eosio_assert( from->balance.amount >= quantity.amount, BLANCE_NOT_ENOUGH );

        Approves approveobj(_self, owner);
        if(approveobj.find(quantity.symbol.name()) != approveobj.end()){
            const auto &approSymIte = approveobj.find(quantity.symbol.name());

            approveobj.modify(approSymIte, owner, [&](auto &a){
                auto approvetoPairIte = a.approved.begin();
                while(approvetoPairIte != a.approved.end()){
                    if(approvetoPairIte->to == spender){
                        approvetoPairIte->value = quantity.amount;
                        break;
                    }
                    approvetoPairIte++;
                }
                if(approvetoPairIte == a.approved.end()){
                    approvetoPair atp;
                    atp.to = spender;
                    atp.value = quantity.amount;
                    a.approved.push_back(atp);
                }
            });
        }else{
            approvetoPair atp;
            atp.to = spender;
            atp.value = quantity.amount;

            approveto at;
            at.approved.push_back(atp);
            at.symbol_name=quantity.symbol;

            approveobj.emplace(owner, [&](auto &a){
                a.symbol_name = quantity.symbol;
                a.approved = at.approved;
            });
        }
    }

    void eosdactoken::balanceof(account_name owner,
                             std::string  symbol){
        symbol_name sn = string_to_symbol(4, symbol.c_str());
        print("balanceof[",symbol.c_str(),"]", get_balance(owner, sn));
    }


    void eosdactoken::allowance(account_name owner,
                              account_name spender,
                              std::string  symbol){
        Approves approveobj(_self, owner);

         if(approveobj.find(string_to_symbol(4, symbol.c_str())) != approveobj.end()){
             const auto &approSymIte = approveobj.get(string_to_symbol(4, symbol.c_str()));
             auto approvetoPairIte = approSymIte.approved.begin();
             while(approvetoPairIte != approSymIte.approved.end()){

                 if(approvetoPairIte->to == spender){
                     print("allowanceof[", account_name(approvetoPairIte->to), "]=", approvetoPairIte->value);
                     return;
                 }
                 approvetoPairIte++;
             }
             if(approvetoPairIte == approSymIte.approved.end()){
                 print("allowanceOf[", account_name(spender), "]=", 0);
             }

         }else{
             print("allowanceOf[", (account_name)spender, "]=", 0);
         }
    }

    void eosdactoken::transferfrom(account_name from,
                                account_name to,
                                asset quantity){
        require_auth(to);

       account_name owner=from;
       account_name spender=to;

       Approves approveobj(_self, from);
       if(approveobj.find(quantity.symbol.name()) != approveobj.end()){
           const auto &approSymIte = approveobj.get(quantity.symbol.name());
           approveobj.modify(approSymIte, owner, [&](auto &a){
               a = approSymIte;
               auto approvetoPairIte = a.approved.begin();
               while(approvetoPairIte != a.approved.end()){

                   if(approvetoPairIte->to == spender){
                       eosio_assert(approvetoPairIte->value>=quantity.amount, NOT_ENOUGH_ALLOWED_OCT_TO_DO_IT);
                       approvetoPairIte->value -= quantity.amount;

                       auto sym = quantity.symbol.name();
                       Stats statstable( _self, sym );
                       const auto& st = statstable.get( sym );
                       require_recipient( to );

                       eosio_assert( quantity.is_valid(),  INVALID_QUANTITY);
                       eosio_assert( quantity.amount > 0, MUST_ISSUE_POSITIVE_QUANTITY);
                       sub_balancefrom( from, quantity, st );
                       add_balancefrom( to, quantity, st);
                       break;
                   }
                   approvetoPairIte++;
               }
               if(approvetoPairIte == a.approved.end()){
                   eosio_assert(false, NOT_ENOUGH_ALLOWED_OCT_TO_DO_IT);
               }
           });
       }else{
           eosio_assert(false, NOT_ENOUGH_ALLOWED_OCT_TO_DO_IT);
       }
    }

    void eosdactoken::create(       account_name           issuer,
                                 asset                  maximum_supply,
                                 uint8_t                issuer_can_freeze,
                                 uint8_t                issuer_can_recall,
                                 uint8_t                issuer_can_whitelist) {
      require_auth( _self );

      auto sym = maximum_supply.symbol;
      eosio_assert( sym.is_valid(), INVALID_SYMBOL_NAME);
      eosio_assert( maximum_supply.is_valid(), INVALID_QUANTITY);
      eosio_assert( maximum_supply.amount>0, TOKEN_MAX_SUPPLY_MUST_POSITIVE) ;

       Stats statstable( _self, sym.name() );
       auto existing = statstable.find( sym.name() );
       eosio_assert( existing == statstable.end(), TOKEN_WITH_SYMBOL_ALREADY_EXISTS);



       statstable.emplace( issuer, [&]( auto& s ) {
          s.supply.symbol = maximum_supply.symbol;
          s.max_supply    = maximum_supply;
          s.issuer        = issuer;
       });
    }
