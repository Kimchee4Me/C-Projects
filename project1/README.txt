To check your program is generating the expected results
using input redirection and diff. The following is an
example that illustrates how we can check the expected
results are generated for the first public test (public01.in).
Notice % represents the Unix prompt.

% a.out < public01.in > my_results
% diff public01.output my_results

If after executing diff you get the prompt back with
no other messages then your program is generating
the expected results for that particular public test.
