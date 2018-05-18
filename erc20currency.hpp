/**

*/

#pragma once
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/multi_index.hpp>
#include <vector>




//namespace oc {
   using std::string;
   using std::array;

    using namespace eosio;
    using boost::container::flat_map;

    struct approvetoPair
    {
        account_name to;
        uint64_t value;
        approvetoPair() {}
        EOSLIB_SERIALIZE(approvetoPair, (to)(value))
    };
    /*
    @abi table approves
    eosiocpp flat_map and map not support now
    */
    struct approveto {
       uint64_t symbol_name;
       std::vector<approvetoPair> approved;
       uint64_t primary_key()const { return symbol_name; }
       EOSLIB_SERIALIZE( approveto, (symbol_name)(approved))
    };

    /*
    @abi action approve
    */
    struct approveact {
        account_name owner;
        account_name spender;
        asset quantity;
        EOSLIB_SERIALIZE(approveact, (owner)(spender)(quantity))
    };

    /*
    @abi action transferfrom
    */
    struct transferfromact {
        account_name from;
        account_name to;
        asset quantity;

        EOSLIB_SERIALIZE(transferfromact, (from)(to)(quantity))
    };

    /*
     *@abi action balanceof
    */
    struct balanceOfAct{
        account_name owner;
        std::string  symbol;
        EOSLIB_SERIALIZE(balanceOfAct, (owner)(symbol))
    };


    struct allowanceOfAct{
        account_name owner;
        account_name spender;
        std::string  symbol;
        EOSLIB_SERIALIZE(allowanceOfAct, (owner)(spender)(symbol))
    };
   /**
    *  This contract enables the creation, issuance, and transfering of many different tokens.
    *
    */
    struct transfersct
    {
       account_name from;
       account_name to;
       asset        quantity;
       string       memo;

       EOSLIB_SERIALIZE( transfersct, (from)(to)(quantity)(memo) )
    };


    struct transferfeesct
    {
       account_name from;
       account_name to;
       asset        quantity;
       account_name tofeeadmin;
       asset        feequantity;
       string       memo;

       EOSLIB_SERIALIZE( transferfeesct, (from)(to)(quantity)(tofeeadmin)(feequantity)(memo) )
    };

    struct issuesct {
       account_name to;
       asset        quantity;
       string       memo;

       EOSLIB_SERIALIZE( issuesct, (to)(quantity)(memo) )
    };

    struct createsct {
       account_name           issuer;
       asset                  maximum_supply;
       uint8_t                issuer_can_freeze     = true;
       uint8_t                issuer_can_recall     = true;
       uint8_t                issuer_can_whitelist  = true;

       /*(issuer_agreement_hash)*/
       EOSLIB_SERIALIZE( createsct, (issuer)(maximum_supply)(issuer_can_freeze)(issuer_can_recall)(issuer_can_whitelist) )
    };

    struct account {
       asset    balance;
       bool     frozen    = false;
       bool     whitelist = true;

       uint64_t primary_key()const { return balance.symbol; }

       EOSLIB_SERIALIZE( account, (balance)(frozen)(whitelist) )
    };


    /*
      @abi table accounts
   */
//    struct accounttb{
//        uint64_t currency;
//        account value;
//    };



    struct currency_stats {
       asset          supply;
       asset          max_supply;
       account_name   issuer;
       bool           can_freeze         = true;
       bool           can_recall         = true;
       bool           can_whitelist      = true;
       bool           is_frozen          = false;
       bool           enforce_whitelist  = false;

       uint64_t primary_key()const { return supply.symbol.name(); }

       EOSLIB_SERIALIZE( currency_stats, (supply)(max_supply)(issuer)(can_freeze)(can_recall)(can_whitelist)(is_frozen)(enforce_whitelist) )
    };

   class currency {
      public:
         currency( account_name contract )
         :_contract(contract)
         { }

         struct fee_schedule {
            uint64_t primary_key()const { return 0; }

            array<extended_asset,7> fee_per_length;
            EOSLIB_SERIALIZE( fee_schedule, (fee_per_length) )
         };

         uint64_t allowanceOf( account_name owner,
         account_name spender,
         symbol_name  symbol){
             approves approveobj(_contract, owner);

             if(approveobj.find(symbol) != approveobj.end()){
                 const auto &approSymIte = approveobj.get(symbol);
                 auto approvetoPairIte = approSymIte.approved.begin();
                 while(approvetoPairIte != approSymIte.approved.end()){

                     if(approvetoPairIte->to == spender){
                         print("allowanceOf[", account_name(approvetoPairIte->to), "]=", approvetoPairIte->value);
                         return approvetoPairIte->value;
                     }
                     approvetoPairIte++;
                 }
                 if(approvetoPairIte == approSymIte.approved.end()){
                     print("allowanceOf[", account_name(spender), "]=", 0);
                 }

             }else{
                 print("allowanceOf[", (account_name)spender, "]=", 0);
             }
             return 0;
         }



         /*
           @abi table stat
        */
         struct stat{
             uint64_t currency;
             currency_stats value;
         };

         typedef eosio::multi_index<N(accounts), account> Accounts;
         typedef eosio::multi_index<N(stat), currency_stats> stats;
         typedef eosio::multi_index<N(approves), approveto> approves;


         asset get_balance( account_name owner, symbol_name symbol )const {
            Accounts t( _contract, owner );
            return t.get(symbol).balance;
         }

         asset get_supply( symbol_name symbol )const {
            Accounts t( _contract, symbol );
            return t.get(symbol).balance;
         }

         static void inline_transfer( account_name from, account_name to, extended_asset amount, string memo = string(), permission_name perm = N(active) ) {
            action act( permission_level( from, perm ), amount.contract, N(transfer), transfersct{from,to,amount,memo} );
            act.send();
         }

         void inline_transfer( account_name from, account_name to, asset amount, string memo = string(), permission_name perm = N(active) ) {
            action act( permission_level( from, perm ), _contract, N(transfer), transfersct{from,to,amount,memo} );
            act.send();
         }

          /// @abi action
          void create(        account_name           issuer,
          asset                  maximum_supply,
          uint8_t                issuer_can_freeze     = true,
          uint8_t                issuer_can_recall     = true,
          uint8_t                issuer_can_whitelist  = true);

          /// @abi action
          void transfer(
          account_name from,
          account_name to,
          asset        quantity,
          string       memo);

          /// @abi action
          void transferfee(       account_name from,
                                  account_name to,
                                  asset        quantity,
                                  account_name tofeeadmin,
                                  asset        feequantity,
                                  string       memo);

          /// @abi action
          void issue(
          account_name to,
          asset        quantity,
          string       memo);

          /// @abi action
          void approve(
          account_name owner,
          account_name spender,
          asset quantity);

          /// @abi action
          void allowancex(
          account_name owner,
          account_name spender,
          std::string  symbol);

          /// @notice send `_value` token to `_to` from `_from` on the condition it is approved by `_from`
          /// @param from The account of the sender
          /// @param to The account of the recipient
          /// @param quantity The amount of token to be transferred
          /// @abi action
          void transferfrom(
          account_name from,
          account_name to,
          asset quantity);

          /// @param owner The account from which the balance will be retrieved
          /// @param symbol
          /// @abi action
          void balanceof(
          account_name owner,
          std::string  symbol);



      private:
          void sub_balance( account_name owner, asset value, const currency_stats& st );

          void add_balance( account_name owner, asset value, const currency_stats& st, account_name ram_payer );

      private:
         account_name _contract;
   };
//}

EOSIO_ABI( currency, (transfer)(create)(issue)(transferfee)(approve)(transferfrom)(balanceof)(allowancex))
