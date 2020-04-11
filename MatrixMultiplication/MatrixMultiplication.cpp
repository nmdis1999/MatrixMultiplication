///
#include <hpx/hpx_init.hpp>
#include <hpx/include/future.hpp>
#include <hpx/include/async.hpp>
#include <hpx/include/parallel_for_each.hpp>
#include <hpx/program_options.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/parallel_generate.hpp>

#include <boost/range/counting_range.hpp>
#include <boost/range/irange.hpp>

#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <functional>
#include <vector>

using vec = std::vector<std::size_t>;
using mat = std::vector<std::vector<size_t> >;

const int MOD =  1000000007;
const int rand_max = 10005;
int rand_wrapper()
{
	return std::rand() % rand_max + 1;
}
int hpx_main(hpx::program_options::variables_map& vm)
{
	uint64_t rows = vm["rows"].as<std::size_t>();
	uint64_t cols = vm["cols"].as<std::size_t>();

	
	vec A(rows * rows);
	vec B(cols * cols);
	size_t n = rows;

	//fill A synchronously and sequentially
	hpx::parallel::generate(hpx::parallel::execution::seq,
                  std::begin(A), std::end(A), &rand_wrapper);
	//fill A synchronously and sequentially
	hpx::parallel::generate(hpx::parallel::execution::seq,
                  std::begin(B), std::end(B), &rand_wrapper);
	
	std::cout << "Matrix A is : \n";
	for (std::size_t i = 0; i < rows; i++)
	{
		for (std::size_t j = 0; j < rows; j++)
		{
			std::cout<<(A[i * n + j]) % MOD<<" ";
		}
		std::cout <<"\n";
	}

	std::cout << "Matrix B is : \n";
	for (std::size_t i = 0; i < rows; i++)
	{
		for (std::size_t j = 0; j < rows; j++)
		{
			std::cout<<(B[i * n + j])%MOD<<" ";
		}
		std::cout <<"\n";
	}


	vec res(n * n);

	using hpx::parallel::for_each;
	using hpx::parallel::execution::par;
	auto range = boost::irange<std::size_t>(0, n);
	for_each(par, boost::begin(range), boost::end(range),
		[&](std::size_t i)
		{
			for(std::size_t j = 0; j < n; j++)
			{
				res[i * n + j] = 0;
				for (std::size_t k = 0; k < n; k++)
				{
					res[i * n + j] += ((A[i * n + k] % MOD) * (B[k * n + j] % MOD));
					res[i * n + j] %= MOD;
				}  
			}

		}
		);

	std::cout << n << "\n";
	std::cout << "Resultant Matrix is : \n";
	for (std:: size_t i = 0; i < n; i++)
	{
		for (std:: size_t j = 0; j < n; j++)
			std::cout << res[i * n + j] % MOD <<" ";
		std::cout <<"\n";
	}
	return hpx::finalize();
}
int main(int argc, char* argv[])
{
	hpx::program_options::options_description
	desc_commandline("Usage: " HPX_APPLICATION_STRING " [options]");

	desc_commandline.add_options()
	( "rows",
		hpx::program_options::value<std::uint64_t>()->default_value(4),
		"Number of rows")
	("cols", 
		hpx::program_options::value<std::uint64_t>()->default_value(4),
		"Number of cols")
	;

    // Initialize and run HPX
	return hpx::init(desc_commandline, argc, argv);
}