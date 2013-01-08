#pragma once
#include <string>
#include <vector>
#include <istream>
#include <ostream>
#include <algorithm>
#include <Windows.h>
#include "pe_exception.h"

#define Is2power(x) (!(x & (x - 1)))
#define ALIGN_DOWN(x, align) (x & ~(align - 1))
#define ALIGN_UP(x, align) ((x & (align - 1)) ? ALIGN_DOWN(x, align) + align : x) 

class pe_base
{
public:

#pragma pack(push, 1)
	struct rich_data
	{
		unsigned int number;
		unsigned int version;
		unsigned int times;
	};

	enum data_directory_id
	{
		export_dir = 0,
		import_dir,
		resource_dir,
		exception_dir,
		security_dir,
		reloc_dir,
		debug_dir,
		copyright_dir,
		globalptr_dir,
		tls_dir,
		config_dir,
		bound_import_dir,
		iat_dir,
		delay_import_dir,
		com_descriptor_dir
	};

	struct section
	{
		friend class pe_base;

		section();

		void set_name(const std::string& name);
		std::string get_name() const;
		section& readable(bool readable);
		section& writeable(bool writeable);
		section& executable(bool executable);
		bool readable() const;
		bool writeable() const;
		bool executable() const;
		IMAGE_SECTION_HEADER header;

		std::string& get_raw_data()
		{
			unmap_virtual();
			return raw_data;
		}

		const std::string& get_raw_data() const
		{
			unmap_virtual();
			return raw_data;
		}

		const std::string& get_virtual_data() const
		{
			map_virtual();
			return raw_data;
		}

	private:
		DWORD raw_size_aligned;
		DWORD virtual_size_aligned;

		void map_virtual() const
		{
			if(old_size == -1 && virtual_size_aligned && virtual_size_aligned > raw_data.length())
			{
				old_size = raw_data.length();
				raw_data.resize(virtual_size_aligned, 0);
			}
		}

		void unmap_virtual() const
		{
			if(old_size != -1)
			{
				raw_data.resize(old_size, 0);
				old_size = -1;
			}
		}

		mutable std::size_t old_size;
		mutable std::string raw_data;
	};
#pragma pack(pop)

	virtual void strip_stub_overlay();
	virtual void fill_stub_overlay(char c);
	virtual bool directory_exists(unsigned long id) const = 0;
	virtual void remove_directory(unsigned long id) = 0;
	virtual DWORD directory_rva(unsigned long id) const = 0;
	virtual void set_directory_rva(unsigned long id, DWORD va) = 0;
	virtual void set_directory_size(unsigned long id, DWORD size) = 0;
	virtual DWORD directory_size(unsigned long id) const = 0;
	virtual unsigned long strip_data_directories(long min_count = 1) = 0;
	virtual DWORD get_file_alignment() const = 0;
	virtual DWORD get_section_alignment() const = 0;
	virtual void realign_all_sections();
	virtual void realign_section(unsigned int number);
	virtual WORD get_number_of_sections() const = 0;
	virtual DWORD get_size_of_image() const = 0;
	virtual DWORD get_number_of_rvas_and_sizes() const = 0;
	virtual void set_ep(DWORD new_ep) = 0;
	virtual DWORD get_ep() const = 0;
	virtual WORD get_pe_characteristics() const = 0;
	virtual void set_pe_characteristics(WORD ch) = 0;
	virtual DWORD get_size_of_headers() const = 0;
	virtual DWORD get_pe_signature() const = 0;
	virtual DWORD get_magic() const = 0;
	virtual WORD get_size_of_optional_header() const = 0;
	virtual void set_file_alignment(DWORD alignment);
	virtual section& section_from_rva(DWORD rva);
	virtual const section& section_from_rva(DWORD rva) const;
	virtual section& add_section(section s);
	virtual void prepare_section(section& s);
	virtual char* section_data_from_rva(section& s, DWORD rva) const;
	virtual const char* section_data_from_rva(const section& s, DWORD rva) const;
	virtual char* section_data_from_rva(DWORD rva);
	virtual const char* section_data_from_rva(DWORD rva) const;

	virtual void set_section_virtual_size(section& s, DWORD vsize);
	virtual void update_image_size();


	virtual std::vector<rich_data> get_rich_data() const;
	virtual void rebuild_pe(bool strip_dos_header = false);
	virtual void rebuild_pe(std::ostream& out, bool strip_dos_header = false);


	virtual DWORD va_to_rva(DWORD va) const = 0;
	virtual DWORD va_to_rva(ULONGLONG va) const = 0;
	virtual DWORD rva_to_va_32(DWORD rva) const = 0;
	virtual ULONGLONG rva_to_va_64(DWORD rva) const = 0;
	virtual void rva_to_va(DWORD rva, DWORD& va) const;
	virtual void rva_to_va(DWORD rva, ULONGLONG& va) const;


	virtual DWORD get_image_base_32() const = 0;
	virtual ULONGLONG get_image_base_64() const = 0;
	virtual void get_image_base(DWORD& base) const;
	virtual void get_image_base(ULONGLONG& base) const;

	virtual WORD get_characteristics() const = 0;
	virtual void set_characteristics(WORD ch) = 0;
	virtual void clear_characteristics_flags(WORD flags);
	virtual void set_characteristics_flags(WORD flags);
	virtual bool check_characteristics_flag(WORD flag);
	
	virtual WORD get_subsystem() const = 0;
	virtual bool is_console() const;
	virtual bool is_gui() const;

	virtual bool has_imports() const;
	virtual bool has_exports() const;
	virtual bool has_resources() const;
	virtual bool has_security() const;
	virtual bool has_reloc() const;
	virtual bool has_tls() const;
	virtual bool has_config() const;
	virtual bool has_bound_import() const;
	virtual bool has_delay_import() const;
	virtual bool is_dotnet() const;

	virtual void set_heap_size_commit(DWORD size) = 0;
	virtual void set_heap_size_commit(ULONGLONG size) = 0;
	virtual void set_heap_size_reserve(DWORD size) = 0;
	virtual void set_heap_size_reserve(ULONGLONG size) = 0;
	virtual void set_stack_size_commit(DWORD size) = 0;
	virtual void set_stack_size_commit(ULONGLONG size) = 0;
	virtual void set_stack_size_reserve(DWORD size) = 0;
	virtual void set_stack_size_reserve(ULONGLONG size) = 0;

	virtual DWORD get_heap_size_commit_32() const = 0;
	virtual ULONGLONG get_heap_size_commit_64() const = 0;
	virtual DWORD get_heap_size_reserve_32() const = 0;
	virtual ULONGLONG get_heap_size_reserve_64() const = 0;
	virtual DWORD get_stack_size_commit_32() const = 0;
	virtual ULONGLONG get_stack_size_commit_64() const = 0;
	virtual DWORD get_stack_size_reserve_32() const = 0;
	virtual ULONGLONG get_stack_size_reserve_64() const = 0;

	virtual void get_heap_size_commit(DWORD& size) const;
	virtual void get_heap_size_commit(ULONGLONG& size) const;
	virtual void get_heap_size_reserve(DWORD& size) const;
	virtual void get_heap_size_reserve(ULONGLONG& size) const;
	virtual void get_stack_size_commit(DWORD& size) const;
	virtual void get_stack_size_commit(ULONGLONG& size) const;
	virtual void get_stack_size_reserve(DWORD& size) const;
	virtual void get_stack_size_reserve(ULONGLONG& size) const;

	virtual char* section_data_from_va(section& s, DWORD va) const;
	virtual const char* section_data_from_va(const section& s, DWORD va) const;
	virtual char* section_data_from_va(DWORD va);
	virtual const char* section_data_from_va(DWORD va) const;
	virtual char* section_data_from_va(section& s, ULONGLONG va) const;
	virtual const char* section_data_from_va(const section& s, ULONGLONG va) const;
	virtual char* section_data_from_va(ULONGLONG va);
	virtual const char* section_data_from_va(ULONGLONG va) const;
	virtual section& section_from_va(DWORD va);
	virtual section& section_from_va(ULONGLONG va);
	virtual const section& section_from_va(DWORD va) const;
	virtual const section& section_from_va(ULONGLONG va) const;


	virtual inline const IMAGE_DOS_HEADER& pe_base::get_dos_header() const
	{
		return dos_header_;
	}

	virtual inline IMAGE_DOS_HEADER& pe_base::get_dos_header()
	{
		return dos_header_;
	}

	virtual inline LONG pe_base::get_pe_header_start() const
	{
		return dos_header_.e_lfanew;
	}

	virtual inline const std::string& pe_base::get_stub_overlay() const
	{
		return rich_overlay_;
	}

	virtual inline std::vector<pe_base::section>& pe_base::get_image_sections()
	{
		return sections_;
	}

	virtual inline const std::vector<pe_base::section>& pe_base::get_image_sections() const
	{
		return sections_;
	}
	virtual ~pe_base()
	{}

protected:
	IMAGE_DOS_HEADER dos_header_;
	std::string rich_overlay_;
	std::vector<section> sections_;
	std::size_t ptr_to_section_data_;

	virtual void read_dos_header(std::istream& file);
	virtual void read_pe(std::istream& file);

	virtual void set_number_of_sections(WORD number) = 0;
	virtual void set_size_of_image(DWORD number) = 0;
	virtual void set_size_of_headers(DWORD size) = 0;
	virtual void set_size_of_optional_header(WORD size) = 0;
	virtual char* get_nt_headers_ptr() = 0;
	virtual unsigned long get_sizeof_nt_header() const = 0;
	virtual unsigned long get_sizeof_opt_headers() const = 0;
	virtual void set_file_alignment_unchecked(DWORD alignment) = 0;
	virtual void set_base_of_code(DWORD base) = 0;
	virtual DWORD get_needed_magic() const = 0;
};
