#pragma once
#include <exception>
#include <stdexcept>

class pe_exception : public std::runtime_error
{
public:
	enum exception_id
	{
		null,
		bad_pe_file,
		bad_dos_header,
		image_nt_headers_not_found,
		error_reading_image_nt_headers,
		error_reading_data_directories,
		pe_signature_incorrect,
		incorrect_number_of_rva_and_sizes,
		error_changing_section_va,
		section_number_incorrect,
		section_table_incorrect,
		incorrect_section_alignment,
		incorrect_file_alignment,
		incorrect_size_of_image,
		image_section_headers_not_found,
		zero_section_sizes,
		section_incorrect_addr_or_size,
		section_not_found,
		image_section_data_not_found,
		no_section_found,
		image_section_table_incorrect,
		directory_not_exists,
		rva_not_exists,
		error_reading_section_header,
		error_reading_overlay
	};
	
	explicit pe_exception(const char* text, exception_id id = null)
		:std::runtime_error(text), id_(id)
	{}
	
	explicit pe_exception(const std::string& text, exception_id id = null)
		:std::runtime_error(text), id_(id)
	{}

	virtual exception_id get_id() const
	{
		return id_;
	}

	virtual ~pe_exception() throw()
	{}

protected:
	exception_id id_;
};
