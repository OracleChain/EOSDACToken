/**

*/

#pragma once
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/multi_index.hpp>
#include <vector>

using std::string;
using std::array;

using namespace eosio;
using boost::container::flat_map;

struct approvetoPair
{
    account_name to;
    int64_t value;
    approvetoPair() {}
    EOSLIB_SERIALIZE(approvetoPair, (to)(value))
};

/*
@abi table approves
*/
struct approveto {
   symbol_type symbol_name;
   std::vector<approvetoPair> approved;
   uint64_t primary_key()const { return symbol_name.name(); }
   EOSLIB_SERIALIZE( approveto, (symbol_name)(approved))
};

/*
@abi table accounts
*/
struct account {
   asset     balance;
   uint64_t primary_key()const { return balance.symbol.name(); }
   EOSLIB_SERIALIZE( account, (balance))
};

/*
@abi table stats
*/
struct curstats {
   asset          supply;
   asset          max_supply;
   account_name   issuer;

   uint64_t primary_key()const { return supply.symbol.name(); }
   EOSLIB_SERIALIZE( curstats, (supply)(max_supply)(issuer))
};

class eosdactoken : public contract {
  public:
     eosdactoken( account_name self )
     :contract(self)
     {}

     struct fee_schedule {
        uint64_t primary_key()const { return 0; }

        array<extended_asset,7> fee_per_length;
        EOSLIB_SERIALIZE( fee_schedule, (fee_per_length) )
     };

     uint64_t allowanceOf( account_name owner,
     account_name spender,
     symbol_name  symbol){
         Approves approveobj(get_self(), owner);

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

     typedef eosio::multi_index<N(accounts), account> Accounts;
     typedef eosio::multi_index<N(stats), curstats> Stats;
     typedef eosio::multi_index<N(approves), approveto> Approves;


     asset get_balance( account_name owner, symbol_name symbol )const {
        Accounts t( _self, owner );
        return t.get(symbol).balance;
     }


     asset get_supply( symbol_name symbol )const {
        Stats statstable( _self, symbol );
        const auto& st = statstable.get( symbol );
        return st.supply;
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
      void allowance(
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
      void sub_balance( account_name owner, asset value, const curstats& st );

      void add_balance( account_name owner, asset value, const curstats& st, account_name ram_payer );

      void sub_balancefrom( account_name owner, asset value, const curstats& st );

      void add_balancefrom( account_name owner, asset value, const curstats& st);


};


