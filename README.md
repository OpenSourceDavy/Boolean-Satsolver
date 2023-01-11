# Boolean-Satsolver
the Dpll-SAT solver to determin a boolean expression can be solved or not, Conbined with Tseitin-Transformation, Boolean Constraint Propagation, PLP algorithm to boost efficiency.

## Examples:
input:  (-((B * (-A * C) * (-C)) * (C * B)))*B * ( B) * (C + A * C) * (B) * --(B * -(A) ) * (C * A)

output:  Unsat
