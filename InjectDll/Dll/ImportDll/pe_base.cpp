#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <istream>
#include <ostream>
#include <algorithm>
#include <Windows.h>
#include "pe_exception.h"
#include "pe_base.h"


pe_base::section::section()
	:raw_size_aligned(0), virtual_size_aligned(0), old_size(-1)
{
	memset(&header, 0, sizeof(IMAGE_SECTION_HEADER));
}

void pe_base::section::set_name(const std::string& name)
{
	memset(header.Name, 0, sizeof(header.Name));
	memcpy(header.Name, name.c_str(), std::min<size_t>(name.length(), sizeof(header.Name)));
}

std::string pe_base::section::get_name() const
{
	char buf[9] = {0};
	memcpy(buf, header.Name, 8);
	return std::string(buf);
}

pe_base::section& pe_base::section::readable(bool readable)
{
	if(readable)
		header.Characteristics |= IMAGE_SCN_MEM_READ;
	else
		header.Characteristics &= ~IMAGE_SCN_MEM_READ;

	return *this;
}

pe_base::section& pe_base::section::writeable(bool writeable)
{
	if(writeable)
		header.Characteristics |= IMAGE_SCN_MEM_WRITE;
	else
		header.Characteristics &= ~IMAGE_SCN_MEM_WRITE;

	return *this;
}

pe_base::section& pe_base::section::executable(bool executable)
{
	if(executable)
		header.Characteristics |= IMAGE_SCN_MEM_EXECUTE;
	else
		header.Characteristics &= ~IMAGE_SCN_MEM_EXECUTE;

	return *this;
}

bool pe_base::section::readable() const
{
	return (header.Characteristics & IMAGE_SCN_MEM_READ) != 0;
}

bool pe_base::section::writeable() const
{
	return (header.Characteristics & IMAGE_SCN_MEM_WRITE) != 0;
}

bool pe_base::section::executable() const
{
	return (header.Characteristics & IMAGE_SCN_MEM_EXECUTE) != 0;
}

void pe_base::strip_stub_overlay()
{
	rich_overlay_.clear();
}

void pe_base::fill_stub_overlay(char c)
{
	if(rich_overlay_.length())
		rich_overlay_.assign(rich_overlay_.length(), c);
}



void pe_base::realign_all_sections()
{
	for(unsigned int i = 0; i < sections_.size(); i++)
		realign_section(i);
}

void pe_base::realign_section(unsigned int number)
{
	if(sections_.size() <= number)
		throw pe_exception("Section not found", pe_exception::section_not_found);

	std::vector<section>::iterator it = sections_.begin() + number;

	std::size_t strip = 0;
	for(std::size_t i = (*it).get_raw_data().length(); i >= 1; i--)
	{
		if((*it).get_raw_data()[i - 1] == 0)
			strip++;
		else
			break;
	}

	(*it).raw_size_aligned = static_cast<DWORD>(ALIGN_UP((*it).get_raw_data().length() - strip, get_file_alignment()));

	if(it == sections_.end() - 1)
	{
		(*it).header.SizeOfRawData = static_cast<DWORD>((*it).get_raw_data().length() - strip);
		(*it).get_raw_data().resize((*it).get_raw_data().length() - strip, 0);
	}
	else
	{
		(*it).header.SizeOfRawData = (*it).raw_size_aligned;
		(*it).get_raw_data().resize( (*it).raw_size_aligned, 0);
	}
}

void pe_base::set_file_alignment(DWORD alignment)
{
	if(alignment < 512)
		throw pe_exception("File alignment can't be less than 512", pe_exception::incorrect_file_alignment);

	if(!Is2power(alignment))
		throw pe_exception("File alignment must be a power of 2", pe_exception::incorrect_file_alignment);

	if(alignment > get_section_alignment())
		throw pe_exception("File alignment must be <= section alignment", pe_exception::incorrect_file_alignment);

	set_file_alignment_unchecked(alignment);
}


pe_base::section& pe_base::section_from_rva(DWORD rva)
{
	for(std::vector<section>::iterator i = sections_.begin(); i != sections_.end(); i++)
	{
		if(rva >= (*i).header.VirtualAddress && rva < (*i).header.VirtualAddress + (*i).virtual_size_aligned)
			return *i;
	}

	throw pe_exception("No section found by presented address", pe_exception::no_section_found);
}

const pe_base::section& pe_base::section_from_rva(DWORD rva) const
{
	for(std::vector<section>::const_iterator i = sections_.begin(); i != sections_.end(); i++)
	{
		if(rva >= (*i).header.VirtualAddress && rva < (*i).header.VirtualAddress + (*i).virtual_size_aligned)
			return *i;
	}

	throw pe_exception("No section found by presented address", pe_exception::no_section_found);
}

void pe_base::set_section_virtual_size(section& s, DWORD vsize)
{
	for(std::vector<section>::const_iterator i = sections_.begin(); i != sections_.end() - 1; i++)
	{
		if(&s == &(*i))
			throw pe_exception("Can't change address of any section, except last one", pe_exception::error_changing_section_va);
	}

	if(vsize == 0)
	{
		s.virtual_size_aligned = ALIGN_UP(s.header.SizeOfRawData, get_section_alignment());
		s.header.Misc.VirtualSize = s.header.SizeOfRawData;
	}
	else
	{
		s.virtual_size_aligned = ALIGN_UP(vsize, get_section_alignment());
		s.header.Misc.VirtualSize = s.virtual_size_aligned;
	}

	if(&s == &(*(sections_.end() - 1)))
		update_image_size();
}

void pe_base::update_image_size()
{
	set_size_of_image(sections_.back().header.VirtualAddress + sections_.back().virtual_size_aligned);
}

void pe_base::prepare_section(section& s)
{
	s.header.SizeOfRawData = static_cast<DWORD>(ALIGN_UP(s.get_raw_data().length(), get_file_alignment()));
	s.raw_size_aligned = s.header.SizeOfRawData;

	if(!s.header.SizeOfRawData && !s.header.Misc.VirtualSize)
		throw pe_exception("Virtual and Physical sizes of section can't be 0 at the same time", pe_exception::zero_section_sizes);

	if(s.header.Misc.VirtualSize == 0)
	{
		s.virtual_size_aligned = ALIGN_UP(s.header.SizeOfRawData, get_section_alignment());
		s.header.Misc.VirtualSize = s.header.SizeOfRawData;
	}
	else
	{
		s.virtual_size_aligned = std::min<DWORD>(ALIGN_UP(s.header.SizeOfRawData, get_file_alignment()),  ALIGN_UP(s.header.Misc.VirtualSize, get_section_alignment()));
	}
}


pe_base::section& pe_base::add_section(section s)
{
	prepare_section(s);

	s.header.VirtualAddress = ALIGN_UP(sections_.back().header.VirtualAddress + sections_.back().virtual_size_aligned, get_section_alignment());

	sections_.push_back(s);
	set_number_of_sections(get_number_of_sections() + 1);
	set_size_of_image(get_size_of_image() + s.virtual_size_aligned);
	return sections_.back();
}


bool pe_base::has_imports() const
{
	return directory_exists(IMAGE_DIRECTORY_ENTRY_IMPORT);
}

bool pe_base::has_exports() const
{
	return directory_exists(IMAGE_DIRECTORY_ENTRY_EXPORT);
}

bool pe_base::has_resources() const
{
	return directory_exists(IMAGE_DIRECTORY_ENTRY_RESOURCE);
}

bool pe_base::has_security() const
{
	return directory_exists(IMAGE_DIRECTORY_ENTRY_SECURITY);
}

bool pe_base::has_reloc() const
{
	return directory_exists(IMAGE_DIRECTORY_ENTRY_BASERELOC) && !(get_characteristics() & IMAGE_FILE_RELOCS_STRIPPED);
}

bool pe_base::has_tls() const
{
	return directory_exists(IMAGE_DIRECTORY_ENTRY_TLS);
}

bool pe_base::has_config() const
{
	return directory_exists(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);
}

bool pe_base::has_bound_import() const
{
	return directory_exists(IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT);
}

bool pe_base::has_delay_import() const
{
	return directory_exists(IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT);
}

bool pe_base::is_dotnet() const
{
	return directory_exists(IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR);
}


char* pe_base::section_data_from_rva(section& s, DWORD rva) const
{
	if(rva >= s.header.VirtualAddress && rva < s.header.VirtualAddress + s.virtual_size_aligned)
		return &s.get_raw_data()[rva - s.header.VirtualAddress];

	throw pe_exception("RVA not found inside section", pe_exception::rva_not_exists);
}

const char* pe_base::section_data_from_rva(const section& s, DWORD rva) const
{
	if(rva >= s.header.VirtualAddress && rva < s.header.VirtualAddress + s.virtual_size_aligned)
		return s.get_raw_data().data() + rva - s.header.VirtualAddress;

	throw pe_exception("RVA not found inside section", pe_exception::rva_not_exists);
}

char* pe_base::section_data_from_rva(DWORD rva)
{
	section& s = section_from_rva(rva);
	return &s.get_raw_data()[rva - s.header.VirtualAddress];
}

const char* pe_base::section_data_from_rva(DWORD rva) const
{
	const section& s = section_from_rva(rva);
	return s.get_raw_data().data() + rva - s.header.VirtualAddress;
}

std::vector<pe_base::rich_data> pe_base::get_rich_data() const
{
	std::vector<rich_data> ret;

	const char* begin = &rich_overlay_[0] + sizeof(IMAGE_DOS_HEADER);

	bool found = false;

	while(true)
	{
		if(*reinterpret_cast<const DWORD*>(begin) == 0x68636952)
		{
			found = true;
			break;
		}

		begin++;

		if(reinterpret_cast<unsigned int>(begin) - reinterpret_cast<unsigned int>(rich_overlay_.data()) > static_cast<unsigned int>(dos_header_.e_lfanew - sizeof(DWORD) * 2))
			break;
	}

	if(found)
	{
		DWORD xorkey = *reinterpret_cast<const DWORD*>(begin + sizeof(DWORD));
		begin = &rich_overlay_[0] + sizeof(IMAGE_DOS_HEADER);

		found = false;

		while(true)
		{
			if((*reinterpret_cast<const DWORD*>(begin) ^ xorkey) == 'SnaD')
			{
				found = true;
				break;
			}

			begin++;

			if(reinterpret_cast<unsigned int>(begin) - reinterpret_cast<unsigned int>(rich_overlay_.data()) > static_cast<unsigned int>(dos_header_.e_lfanew - sizeof(DWORD) * 2))
				break;
		}

		if(found)
		{
			begin += sizeof(DWORD) * 3;
			while(true)
			{
				if(reinterpret_cast<unsigned int>(begin) - reinterpret_cast<unsigned int>(rich_overlay_.data()) >= static_cast<unsigned int>(dos_header_.e_lfanew - sizeof(DWORD) * 2))
					break;

				begin += sizeof(DWORD);
				if(*reinterpret_cast<const DWORD*>(begin) == 0x68636952)
					break;

				rich_data data;
				data.number = (*reinterpret_cast<const DWORD*>(begin) ^ xorkey) >> 16;
				data.version = (*reinterpret_cast<const DWORD*>(begin) ^ xorkey) & 0xFFFF;

				begin += sizeof(DWORD);

				data.times = (*reinterpret_cast<const DWORD*>(begin) ^ xorkey);

				ret.push_back(data);
			}
		}
	}

	return ret;
}


void pe_base::rebuild_pe(bool strip_dos_header)
{
	dos_header_.e_lfanew = sizeof(IMAGE_DOS_HEADER) + static_cast<DWORD>(rich_overlay_.size());

	if(strip_dos_header)
	{
		set_base_of_code(8 * sizeof(WORD));
		strip_stub_overlay();
	}

	ptr_to_section_data_ = ALIGN_UP((strip_dos_header ? 8 * sizeof(WORD) : sizeof(IMAGE_DOS_HEADER)) + get_sizeof_nt_header() + rich_overlay_.size()
		- sizeof(IMAGE_DATA_DIRECTORY) * (IMAGE_NUMBEROF_DIRECTORY_ENTRIES - get_number_of_rvas_and_sizes())
		+ sections_.size() * sizeof(IMAGE_SECTION_HEADER), get_file_alignment());

	set_size_of_headers(std::min<DWORD>(static_cast<DWORD>(ptr_to_section_data_), (*sections_.begin()).header.VirtualAddress));
	set_size_of_optional_header(static_cast<WORD>(get_sizeof_opt_headers() - sizeof(IMAGE_DATA_DIRECTORY) * (IMAGE_NUMBEROF_DIRECTORY_ENTRIES - get_number_of_rvas_and_sizes())));

	for(std::vector<section>::iterator it = sections_.begin(); it != sections_.end(); it++)
	{
		(*it).header.PointerToRawData = static_cast<DWORD>(ptr_to_section_data_);
		ptr_to_section_data_ += (*it).raw_size_aligned;
	}
}

void pe_base::rebuild_pe(std::ostream& out, bool strip_dos_header)
{
	out.exceptions(0);
	out.clear();
	out.seekp(0);

	rebuild_pe(strip_dos_header);

	out.write(reinterpret_cast<char*>(&dos_header_), strip_dos_header ? 8 * sizeof(WORD) : sizeof(IMAGE_DOS_HEADER));
	if(rich_overlay_.size())
		out.write(rich_overlay_.data(), rich_overlay_.size());

	out.write(get_nt_headers_ptr(), get_sizeof_nt_header() - sizeof(IMAGE_DATA_DIRECTORY) * (IMAGE_NUMBEROF_DIRECTORY_ENTRIES - get_number_of_rvas_and_sizes()));

	for(std::vector<section>::iterator it = sections_.begin(); it != sections_.end(); it++)
		out.write(reinterpret_cast<char*>(&(*it).header), sizeof(IMAGE_SECTION_HEADER));

	for(std::vector<section>::iterator it = sections_.begin(); it != sections_.end(); it++)
	{
		std::streamoff cpos = out.tellp();
		for(unsigned int i = 0; i < (*it).header.PointerToRawData - cpos; i++)
			out.put(0);

		out.write((*it).get_raw_data().data(), (*it).get_raw_data().length());
	}
}

void pe_base::read_dos_header(std::istream& file)
{
	file.seekg(0);
	if(file.bad() || file.eof())
		throw pe_exception("PE file stream is bad or closed.", pe_exception::bad_pe_file);

	file.read(reinterpret_cast<char*>(&dos_header_), sizeof(IMAGE_DOS_HEADER));
	if(file.bad() || file.eof())
		throw pe_exception("Unable to read IMAGE_DOS_HEADER", pe_exception::bad_dos_header);

	if(dos_header_.e_magic != 'ZM')
		throw pe_exception("IMAGE_DOS_HEADER signature is incorrect", pe_exception::bad_dos_header);
}

void pe_base::read_pe(std::istream& file)
{
	file.seekg(0, std::ios::end);
	std::streamoff filesize = file.tellg();
	file.seekg(0);

	if((dos_header_.e_lfanew % sizeof(DWORD)) != 0)
		throw pe_exception("PE header is not DWORD-aligned", pe_exception::bad_dos_header);

	file.seekg(dos_header_.e_lfanew);
	if(file.bad() || file.fail())
		throw pe_exception("Cannot reach IMAGE_NT_HEADERS", pe_exception::image_nt_headers_not_found);

	file.read(get_nt_headers_ptr(), get_sizeof_nt_header() - sizeof(IMAGE_DATA_DIRECTORY) * 16);
	if(file.bad() || file.eof())
		throw pe_exception("Error reading IMAGE_NT_HEADERS", pe_exception::error_reading_image_nt_headers);

	if(get_pe_signature() != 'EP')
		throw pe_exception("Incorrect PE signature", pe_exception::pe_signature_incorrect);

	if(get_number_of_rvas_and_sizes() > IMAGE_NUMBEROF_DIRECTORY_ENTRIES)
		throw pe_exception("Incorrect NumberOfRvaAndSizes", pe_exception::incorrect_number_of_rva_and_sizes);

	if(get_number_of_rvas_and_sizes() > 0)
	{
		file.read(get_nt_headers_ptr() + (get_sizeof_nt_header() - sizeof(IMAGE_DATA_DIRECTORY) * 16), sizeof(IMAGE_DATA_DIRECTORY) * get_number_of_rvas_and_sizes());
		if(file.bad() || file.eof())
			throw pe_exception("Error reading DATA_DIRECTORY headers", pe_exception::error_reading_data_directories);
	}

	if(get_number_of_sections() < 1 || get_number_of_sections() > 0x60)
		throw pe_exception("Incorrect number of sections", pe_exception::section_number_incorrect);

	if(get_magic() != get_needed_magic())
		throw pe_exception("Incorrect PE signature", pe_exception::pe_signature_incorrect);

	if(!Is2power(get_section_alignment()))
		throw pe_exception("Incorrect section alignment", pe_exception::incorrect_section_alignment);

	if(!Is2power(get_file_alignment()) || get_file_alignment() < 512 || get_file_alignment() > get_section_alignment())
		throw pe_exception("Incorrect file alignment", pe_exception::incorrect_file_alignment);

	if(ALIGN_UP(get_size_of_image(), get_section_alignment()) == 0)
		throw pe_exception("Incorrect size of image", pe_exception::incorrect_size_of_image);

	if(dos_header_.e_lfanew > sizeof(IMAGE_DOS_HEADER))
	{
		rich_overlay_.resize(dos_header_.e_lfanew - sizeof(IMAGE_DOS_HEADER));
		file.seekg(sizeof(IMAGE_DOS_HEADER));
		file.read(&rich_overlay_[0], dos_header_.e_lfanew - sizeof(IMAGE_DOS_HEADER));
		if(file.bad() || file.eof())
			throw pe_exception("Error reading 'Rich' & 'DOS stub' overlay", pe_exception::error_reading_overlay);
	}

	DWORD first_section = dos_header_.e_lfanew + get_size_of_optional_header() + sizeof(IMAGE_FILE_HEADER) + sizeof(DWORD) /* Signature */;

	file.seekg(first_section);
	if(file.bad() || file.fail())
		throw pe_exception("Cannot reach section headers", pe_exception::image_section_headers_not_found);

	for(int i = 0; i < get_number_of_sections(); i++)
	{
		section s;
		file.read(reinterpret_cast<char*>(&s.header), sizeof(IMAGE_SECTION_HEADER));
		if(file.bad() || file.eof())
			throw pe_exception("Error reading section header" , pe_exception::error_reading_section_header);

		std::streamoff next_sect = file.tellg();

		if(!s.header.SizeOfRawData && !s.header.Misc.VirtualSize)
			throw pe_exception("Virtual and Physical sizes of section can't be 0 at the same time", pe_exception::zero_section_sizes);

		if(s.header.SizeOfRawData != 0)
		{
			if(s.header.VirtualAddress + ALIGN_UP(s.header.Misc.VirtualSize, get_section_alignment()) > ALIGN_UP(get_size_of_image(), get_section_alignment())
				||
				ALIGN_DOWN(s.header.PointerToRawData, get_file_alignment()) + s.header.SizeOfRawData > filesize)
				throw pe_exception("Incorrect section address or size", pe_exception::section_incorrect_addr_or_size);

			file.seekg(ALIGN_DOWN(s.header.PointerToRawData, get_file_alignment()));
			if(file.bad() || file.fail())
				throw pe_exception("Cannot reach section data", pe_exception::image_section_data_not_found);

			if(s.header.Misc.VirtualSize == 0)
			{
				s.virtual_size_aligned = ALIGN_UP(s.header.SizeOfRawData, get_section_alignment());

				if(s.header.VirtualAddress + s.virtual_size_aligned > ALIGN_UP(get_size_of_image(), get_section_alignment()))
					throw pe_exception("Incorrect section address or size", pe_exception::section_incorrect_addr_or_size);
			}
			else
			{
				s.virtual_size_aligned = ALIGN_UP(s.header.Misc.VirtualSize, get_section_alignment());

				if(s.header.VirtualAddress + s.virtual_size_aligned > ALIGN_UP(get_size_of_image(), get_section_alignment()))
					throw pe_exception("Incorrect section address or size", pe_exception::section_incorrect_addr_or_size);
			}

			s.raw_size_aligned = ALIGN_UP(s.header.SizeOfRawData, get_file_alignment());

			s.get_raw_data().resize(s.header.SizeOfRawData);
			file.read(&s.get_raw_data()[0], s.header.SizeOfRawData);
			if(file.bad() || file.fail())
				throw pe_exception("Error reading section data", pe_exception::image_section_data_not_found);
		}
		else
		{
			if(s.header.Misc.VirtualSize == 0)
				throw pe_exception("Section table is incorrect", pe_exception::image_section_table_incorrect);

			s.raw_size_aligned = 0;
			s.virtual_size_aligned = ALIGN_UP(s.header.Misc.VirtualSize, get_section_alignment());
		}

		sections_.push_back(s);

		file.seekg(next_sect);
	}

	if(sections_.size() >= 2)
	{
		for(std::vector<section>::const_iterator i = sections_.begin() + 1; i != sections_.end(); i++)
		{
			if((*i).header.VirtualAddress != (*(i - 1)).header.VirtualAddress + (*(i - 1)).virtual_size_aligned)
				throw pe_exception("Section table is incorrect", pe_exception::image_section_table_incorrect);
		}
	}
}


void pe_base::clear_characteristics_flags(WORD flags)
{
	set_characteristics(get_characteristics() & ~flags);
}

void pe_base::set_characteristics_flags(WORD flags)
{
	set_characteristics(get_characteristics() | flags);
}

bool pe_base::check_characteristics_flag(WORD flag)
{
	return (get_characteristics() & flag) ? true : false;
}

bool pe_base::is_console() const
{
	return (get_subsystem() & IMAGE_SUBSYSTEM_WINDOWS_CUI) ? true : false;
}

bool pe_base::is_gui() const
{
	return (get_subsystem() & IMAGE_SUBSYSTEM_WINDOWS_GUI) ? true : false;
}

char* pe_base::section_data_from_va(section& s, DWORD va) const
{
	return section_data_from_rva(s, va_to_rva(va));
}

const char* pe_base::section_data_from_va(const section& s, DWORD va) const
{
	return section_data_from_rva(s, va_to_rva(va));
}

char* pe_base::section_data_from_va(DWORD va)
{
	return section_data_from_rva(va_to_rva(va));
}

const char* pe_base::section_data_from_va(DWORD va) const
{
	return section_data_from_rva(va_to_rva(va));
}

char* pe_base::section_data_from_va(section& s, ULONGLONG va) const
{
	return section_data_from_rva(s, va_to_rva(va));
}

const char* pe_base::section_data_from_va(const section& s, ULONGLONG va) const
{
	return section_data_from_rva(s, va_to_rva(va));
}

char* pe_base::section_data_from_va(ULONGLONG va)
{
	return section_data_from_rva(va_to_rva(va));
}

const char* pe_base::section_data_from_va(ULONGLONG va) const
{
	return section_data_from_rva(va_to_rva(va));
}


pe_base::section& pe_base::section_from_va(DWORD va)
{
	return section_from_rva(va_to_rva(va));
}

pe_base::section& pe_base::section_from_va(ULONGLONG va)
{
	return section_from_rva(va_to_rva(va));
}

const pe_base::section& pe_base::section_from_va(DWORD va) const
{
	return section_from_rva(va_to_rva(va));
}

const pe_base::section& pe_base::section_from_va(ULONGLONG va) const
{
	return section_from_rva(va_to_rva(va));
}

void pe_base::rva_to_va(DWORD rva, DWORD& va) const
{
	va = rva_to_va_32(rva);
}

void pe_base::rva_to_va(DWORD rva, ULONGLONG& va) const
{
	va = rva_to_va_64(rva);
}

void pe_base::get_image_base(DWORD& base) const
{
	base = get_image_base_32();
}
void pe_base::get_image_base(ULONGLONG& base) const
{
	base = get_image_base_64();
}


void pe_base::get_heap_size_commit(DWORD& size) const
{
	size = get_heap_size_commit_32();
}

void pe_base::get_heap_size_commit(ULONGLONG& size) const
{
	size = get_heap_size_commit_64();
}

void pe_base::get_heap_size_reserve(DWORD& size) const
{
	size = get_heap_size_reserve_32();
}

void pe_base::get_heap_size_reserve(ULONGLONG& size) const
{
	size = get_heap_size_reserve_64();
}

void pe_base::get_stack_size_commit(DWORD& size) const
{
	size = get_stack_size_commit_32();
}

void pe_base::get_stack_size_commit(ULONGLONG& size) const
{
	size = get_stack_size_commit_64();
}

void pe_base::get_stack_size_reserve(DWORD& size) const
{
	size = get_stack_size_reserve_32();
}

void pe_base::get_stack_size_reserve(ULONGLONG& size) const
{
	size = get_stack_size_reserve_64();
}
