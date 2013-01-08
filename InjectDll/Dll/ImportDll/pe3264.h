#pragma once
#include <string>
#include <vector>
#include <istream>
#include <ostream>
#include <algorithm>
#include "pe_exception.h"
#include "pe_base.h"

template<typename NtHeaders, typename OptHeaders, WORD Id, typename BaseSize>
class pe : public pe_base
{
public:
	explicit pe(std::istream& file)
	{
		file.exceptions(0);
		read_dos_header(file);
		read_pe(file);
	}

	virtual ~pe()
	{}
	
	virtual WORD get_pe_characteristics() const
	{
		return nt_headers_.FileHeader.Characteristics;
	}

	virtual void set_pe_characteristics(WORD ch)
	{
		nt_headers_.FileHeader.Characteristics = ch;
	}

	virtual bool directory_exists(unsigned long id) const
	{
		return (nt_headers_.OptionalHeader.NumberOfRvaAndSizes - 1) >= id &&
			nt_headers_.OptionalHeader.DataDirectory[id].VirtualAddress;
	}

	virtual void remove_directory(unsigned long id)
	{
		if(directory_exists(id))
		{
			nt_headers_.OptionalHeader.DataDirectory[id].VirtualAddress = 0;
			nt_headers_.OptionalHeader.DataDirectory[id].Size = 0;

			if(id == IMAGE_DIRECTORY_ENTRY_BASERELOC)
				set_characteristics_flags(IMAGE_FILE_RELOCS_STRIPPED);
			else if(id == IMAGE_DIRECTORY_ENTRY_EXPORT)
				clear_characteristics_flags(IMAGE_FILE_DLL);
		}
	}

	virtual DWORD directory_rva(unsigned long id) const
	{
		if(nt_headers_.OptionalHeader.NumberOfRvaAndSizes <= id)
			throw pe_exception("Specified directory does not exist", pe_exception::directory_not_exists);

		return nt_headers_.OptionalHeader.DataDirectory[id].VirtualAddress;
	}

	virtual void set_directory_rva(unsigned long id, DWORD va)
	{
		if(nt_headers_.OptionalHeader.NumberOfRvaAndSizes <= id)
			throw pe_exception("Specified directory does not exist", pe_exception::directory_not_exists);

		nt_headers_.OptionalHeader.DataDirectory[id].VirtualAddress = va;
	}

	virtual void set_directory_size(unsigned long id, DWORD size)
	{
		if(nt_headers_.OptionalHeader.NumberOfRvaAndSizes <= id)
			throw pe_exception("Specified directory does not exist", pe_exception::directory_not_exists);

		nt_headers_.OptionalHeader.DataDirectory[id].Size = size;
	}

	virtual DWORD directory_size(unsigned long id) const
	{
		if(nt_headers_.OptionalHeader.NumberOfRvaAndSizes <= id)
			throw pe_exception("Specified directory does not exist", pe_exception::directory_not_exists);

		return nt_headers_.OptionalHeader.DataDirectory[id].Size;
	}
	
	virtual DWORD get_image_base_32() const
	{
		return static_cast<DWORD>(nt_headers_.OptionalHeader.ImageBase);
	}

	virtual ULONGLONG get_image_base_64() const
	{
		return static_cast<ULONGLONG>(nt_headers_.OptionalHeader.ImageBase);
	}

	virtual DWORD get_ep() const
	{
		return nt_headers_.OptionalHeader.AddressOfEntryPoint;
	}

	virtual void set_ep(DWORD new_ep)
	{
		nt_headers_.OptionalHeader.AddressOfEntryPoint = new_ep;
	}


	virtual DWORD get_file_alignment() const
	{
		return nt_headers_.OptionalHeader.FileAlignment;
	}

	virtual DWORD get_section_alignment() const
	{
		return nt_headers_.OptionalHeader.SectionAlignment;
	}

	
	virtual DWORD va_to_rva(DWORD va) const
	{
		return static_cast<DWORD>(va - nt_headers_.OptionalHeader.ImageBase);
	}

	virtual DWORD va_to_rva(ULONGLONG va) const
	{
		return static_cast<DWORD>(va - nt_headers_.OptionalHeader.ImageBase);
	}
	
	virtual DWORD rva_to_va_32(DWORD rva) const
	{
		return static_cast<DWORD>(rva + nt_headers_.OptionalHeader.ImageBase);
	}

	virtual ULONGLONG rva_to_va_64(DWORD rva) const
	{
		return static_cast<ULONGLONG>(rva + nt_headers_.OptionalHeader.ImageBase);
	}

	//если сумма image base и e_lfanew вылетает за пределы отведенного загрузчиком адресного пространства, такой файл не грузится.

	virtual unsigned long strip_data_directories(long min_count = 1)
	{
		long i = nt_headers_.OptionalHeader.NumberOfRvaAndSizes - 1;

		for(; i >= 0; i--)
		{
			if(nt_headers_.OptionalHeader.DataDirectory[i].VirtualAddress && i != IMAGE_DIRECTORY_ENTRY_IAT)
				break;

			if(i <= min_count - 2)
				break;
		}

		if(i == IMAGE_NUMBEROF_DIRECTORY_ENTRIES - 1)
			return IMAGE_NUMBEROF_DIRECTORY_ENTRIES;

		return nt_headers_.OptionalHeader.NumberOfRvaAndSizes = i + 1;
	}
	
	virtual void set_heap_size_commit(DWORD size)
	{
		nt_headers_.OptionalHeader.SizeOfHeapCommit = static_cast<BaseSize>(size);
	}

	virtual void set_heap_size_commit(ULONGLONG size)
	{
		nt_headers_.OptionalHeader.SizeOfHeapCommit = static_cast<BaseSize>(size);
	}
	
	virtual void set_heap_size_reserve(DWORD size)
	{
		nt_headers_.OptionalHeader.SizeOfHeapReserve = static_cast<BaseSize>(size);
	}

	virtual void set_heap_size_reserve(ULONGLONG size)
	{
		nt_headers_.OptionalHeader.SizeOfHeapReserve = static_cast<BaseSize>(size);
	}
	
	virtual void set_stack_size_commit(DWORD size)
	{
		nt_headers_.OptionalHeader.SizeOfStackCommit = static_cast<BaseSize>(size);
	}

	virtual void set_stack_size_commit(ULONGLONG size)
	{
		nt_headers_.OptionalHeader.SizeOfStackCommit = static_cast<BaseSize>(size);
	}
	
	virtual void set_stack_size_reserve(DWORD size)
	{
		nt_headers_.OptionalHeader.SizeOfStackReserve = static_cast<BaseSize>(size);
	}

	virtual void set_stack_size_reserve(ULONGLONG size)
	{
		nt_headers_.OptionalHeader.SizeOfStackReserve = static_cast<BaseSize>(size);
	}
	
	virtual DWORD get_heap_size_commit_32() const
	{
		return static_cast<DWORD>(nt_headers_.OptionalHeader.SizeOfHeapCommit);
	}

	virtual ULONGLONG get_heap_size_commit_64() const
	{
		return static_cast<ULONGLONG>(nt_headers_.OptionalHeader.SizeOfHeapCommit);
	}

	virtual DWORD get_heap_size_reserve_32() const
	{
		return static_cast<DWORD>(nt_headers_.OptionalHeader.SizeOfHeapReserve);
	}

	virtual ULONGLONG get_heap_size_reserve_64() const
	{
		return static_cast<ULONGLONG>(nt_headers_.OptionalHeader.SizeOfHeapReserve);
	}

	virtual DWORD get_stack_size_commit_32() const
	{
		return static_cast<DWORD>(nt_headers_.OptionalHeader.SizeOfStackCommit);
	}

	virtual ULONGLONG get_stack_size_commit_64() const
	{
		return static_cast<ULONGLONG>(nt_headers_.OptionalHeader.SizeOfStackCommit);
	}

	virtual DWORD get_stack_size_reserve_32() const
	{
		return static_cast<DWORD>(nt_headers_.OptionalHeader.SizeOfStackReserve);
	}

	virtual ULONGLONG get_stack_size_reserve_64() const
	{
		return static_cast<ULONGLONG>(nt_headers_.OptionalHeader.SizeOfStackReserve);
	}


	virtual WORD get_number_of_sections() const
	{
		return nt_headers_.FileHeader.NumberOfSections;
	}

	virtual DWORD get_size_of_image() const
	{
		return nt_headers_.OptionalHeader.SizeOfImage;
	}

	virtual DWORD get_number_of_rvas_and_sizes() const
	{
		return nt_headers_.OptionalHeader.NumberOfRvaAndSizes;
	}

	virtual WORD get_characteristics() const
	{
		return nt_headers_.FileHeader.Characteristics;
	}

	virtual void set_characteristics(WORD ch)
	{
		nt_headers_.FileHeader.Characteristics = ch;
	}

	virtual DWORD get_size_of_headers() const
	{
		return nt_headers_.OptionalHeader.SizeOfHeaders;
	}

	virtual WORD get_subsystem() const
	{
		return nt_headers_.OptionalHeader.Subsystem;
	}

	virtual WORD get_size_of_optional_header() const
	{
		return nt_headers_.FileHeader.SizeOfOptionalHeader;
	}

	virtual DWORD get_pe_signature() const
	{
		return nt_headers_.Signature;
	}

	virtual DWORD get_magic() const
	{
		return nt_headers_.OptionalHeader.Magic;
	}

protected:
	NtHeaders nt_headers_;

	virtual void set_number_of_sections(WORD number)
	{
		nt_headers_.FileHeader.NumberOfSections = number;
	}

	virtual void set_size_of_image(DWORD size)
	{
		nt_headers_.OptionalHeader.SizeOfImage = size;
	}

	virtual void set_size_of_headers(DWORD size)
	{
		nt_headers_.OptionalHeader.SizeOfHeaders = size;
	}

	virtual void set_size_of_optional_header(WORD size)
	{
		nt_headers_.FileHeader.SizeOfOptionalHeader = size;
	}

	virtual char* get_nt_headers_ptr()
	{
		return reinterpret_cast<char*>(&nt_headers_);
	}
	
	virtual unsigned long get_sizeof_nt_header() const
	{
		return sizeof(NtHeaders);
	}

	virtual unsigned long get_sizeof_opt_headers() const
	{
		return sizeof(OptHeaders);
	}

	virtual void set_file_alignment_unchecked(DWORD alignment) 
	{
		nt_headers_.OptionalHeader.FileAlignment = alignment;
	}

	virtual void set_base_of_code(DWORD base)
	{
		nt_headers_.OptionalHeader.BaseOfCode = 8 * sizeof(WORD);
	}

	virtual DWORD get_needed_magic() const
	{
		return Id;
	}
};


typedef pe<IMAGE_NT_HEADERS32, IMAGE_OPTIONAL_HEADER32, 0x10B, DWORD> pe32;
typedef pe<IMAGE_NT_HEADERS64, IMAGE_OPTIONAL_HEADER64, 0x20B, ULONGLONG> pe64;
