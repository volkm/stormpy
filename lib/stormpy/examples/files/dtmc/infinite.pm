dtmc

module m
    s : [0..2] init 0;
    [] s=0 -> 1/2 : (s'=1) + 1/2 : (s'=2);
    [] s>0 -> true;
endmodule

rewards "r"
    true : 1;
endrewards

label "goal" = s=1;