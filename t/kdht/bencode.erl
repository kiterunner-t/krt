%%% Copyleft (C) KRT, 2013 by kiterunner_t

-module(bencode).

-export([decode/1, encode/1]).


decode(D) ->
  case catch decode(D, []) of
    {'EXIT', _} -> {error, unparse};
    {Res, _}    -> {ok, Res}
  end.


encode(D) ->
  iolist_to_binary(encode_in(D)).


decode(<<$i, T/binary>>, Out) ->
  {Int, T1} = decode_int(T, []),
  decode(T1, [Int | Out]);

decode(<<$l, T/binary>>, Out) ->
  {List, T1} = decode_list(T),
  decode(T1, [List | Out]);

decode(<<$d, T/binary>>, Out) ->
  {Dict, T1} = decode_dict(T, dict:new()),
  decode(T1, [Dict | Out]);

decode(<<$e, T/binary>>, Out) ->
  {Out, T};

decode(<<H, T/binary>>, Out) ->
  {Str, T1} = decode_string(T, [H]),
  decode(T1, [Str | Out]);

decode(<<>>, Out) ->
  Out.


decode_int(<<$e, T/binary>>, Int) ->
  {list_to_integer(lists:reverse(Int)), T};

decode_int(<<H, T/binary>>, Int) ->
  decode_int(T, [H | Int]).


decode_string(<<$:, T/binary>>, LenList) ->
  Len = list_to_integer(lists:reverse(LenList)),
  <<Str:Len/binary, R/binary>> = T,
  {Str, R};

decode_string(<<H, T/binary>>, LenList) ->
  decode_string(T, [H | LenList]).


decode_list(T) ->
  {List, T1} = decode(T, []),
  {lists:reverse(List), T1}.


decode_dict(T, D) ->
  {K, T1} = decode_string(T, []),
  {V, T2} = decode(T1, []),
  {dict:store(K, V, D), T2}.


encode_in(Int) when is_integer(Int) ->
  [$i, term_to_binary(Int), $e];

encode_in(Str) when is_list(Str) ->
  Bin = list_to_binary(Str),
  [term_to_binary(size(Bin)), $:, Bin];

encode_in({list, L}) when is_list(L) ->
  [$l, [encode_in(E) || E <- L], $e];

encode_in({dict, Dict}) when is_dict(Dict) ->
  [$d, [encode_in(K), encode_in(V) || {K, V} <- dict:to_list(Dict)], $e].

