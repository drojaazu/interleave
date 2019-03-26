#include "main.hpp"

const char version[] = "1.0";

u8 width{1}, skip_after{0}, skip_before{0};
std::ostream *outdata = nullptr;
std::ifstream *infiles = nullptr;

int main(int argc, char **argv)
{
	try
	{
		// setup infiles
		u8 optind = process_args(argc, argv);
		u8 file_count = argc - optind;

		if(file_count == 1)
		{
			std::cerr << "Only one input file specified; nothing to do!" << std::endl;
			exit(2);
		}

		if(outdata == nullptr) outdata = &std::cout;

		infiles = new std::ifstream[file_count];
		for(u8 this_file = 0; this_file < file_count; this_file++)
		{
			infiles[this_file] = std::ifstream(argv[optind], std::ios::binary);
			if(!infiles[this_file].good())
			{
				std::cerr << "Error opening file: " << argv[optind] << std::endl;
				delete[] infiles;
				exit(1);
			}
			optind++;
		}

		char this_chunk[width];

		u8 eof_count = 0;

		do
		{
			for(u8 this_file = 0; this_file < file_count; this_file++)
			{
				// already at the end of this file? skip it
				if(!infiles[this_file].good()) continue;
				// check for any read errors that may have occurred
				if(infiles[this_file].bad())
				{
					std::cerr << "Error reading input file!" << std::endl;
					exit(-1);
				}

				// get data...
				if(skip_before)
				{
					infiles[this_file].seekg(skip_before, infiles[this_file].cur);
				}
				if(infiles[this_file].good())
					infiles[this_file].read(this_chunk, width);
				if(skip_after)
				{
					infiles[this_file].seekg(skip_after, infiles[this_file].cur);
				}
				// and if we're now past the end of the file, increase the eof counter
				if(!infiles[this_file].good()) eof_count++;

				outdata->write(this_chunk, infiles[this_file].gcount());
			}
		} while(eof_count < file_count);

		outdata->flush();

		if(outdata != &std::cout) delete outdata;
		delete[] infiles;
	}
	catch(const std::exception &e)
	{
		std::cerr << "Fatal error: " << e.what() << std::endl;
		delete[] infiles;
		exit(-1);
	}
	exit(0);
}

u8 process_args(int argc, char **argv)
{
	// --width -w - number of bytes
	// --output -o - output file (otherwise stdout)
	const char *const short_opts = ":w:o:s:";
	const option long_opts[] = {{"width", required_argument, nullptr, 'w'},
															{"output", required_argument, nullptr, 'o'},
															{"skip-after", required_argument, nullptr, 'a'},
															{"skip-before", required_argument, nullptr, 'b'},
															{"help", no_argument, nullptr, 'h'},
															{nullptr, 0, nullptr, 0}};

	int this_opt;

	while((this_opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) !=
				-1)
	{
		switch(this_opt)
		{
			case 'w':
				// --width -w - number of bytes to interleave
				width = std::stoul(optarg);
				break;
			case 'o':
				// --output -o - output file
				outdata = new std::ofstream(optarg, std::ios::binary);
				break;
			case 'a':
				// --skip-after -a - skip x bytes after each read
				skip_after = std::stoul(optarg);
				break;
			case 'b':
				// --skip-before -b - skip x bytes before each read
				skip_before = std::stoul(optarg);
				break;
			case 'h':
				// --help
				print_help();
				exit(0);
				break;
		}
	}

	if(optind < argc)
	{
		return optind;
	}
	else
	{
		std::cerr << "No input files specified!" << std::endl;
		print_help();
		exit(3);
	}
}

void print_help()
{
	std::cerr << "interleave version " << version << std::endl << std::endl;
	std::cerr << "Usage:" << std::endl;
	std::cerr << "interleave infile1 infile2 ..." << std::endl;
	std::cerr << "Options:" << std::endl;
	std::cerr << "  --width, -w         Number of bytes to interleave; optional; "
							 "default is 2"
						<< std::endl;
	std::cerr
			<< "  --output, -o        Output interleaved file; optional; default "
				 "is stdout"
			<< std::endl;
	std::cerr << "  --skip-before, -b   Number of bytes to skip over before each "
							 "read; optional"
						<< std::endl;
	std::cerr << "  --skip-after, -a    Number of bytes to skip over after each "
							 "read; optional"
						<< std::endl;
	std::cerr << "  --help, -h          Display this text" << std::endl;
}
