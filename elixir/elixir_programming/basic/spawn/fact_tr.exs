defmodule TailRecursive do
    def factorial(n), do: _fact(n,1)
    defp _fact(0, acc), do: acc
    defp _fact(n, acc), do: _fact(n-1, n*acc)
end

IO.puts TailRecursive.factorial(5)