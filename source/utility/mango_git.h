#pragma once
#include <filesystem>

// Mango is a lightweight libgit2 wrapper

// TODO: Use a pimpl so this isn't exposed!
struct git_repository;

namespace mango::git 
{
	template <typename git_struct_t, void (*free_func)(git_struct_t*) >
	class GitType {
	public:
		using git_struct_type = git_struct_t;
		
		template <typename init_func_t, typename... args_t>
		GitType(init_func_t init_func, args_t... args) { if (init_func) init_func(&data_, args...); }
		
		GitType(GitType<git_struct_type, free_func> const&) = delete;

		virtual ~GitType() { free_func(data_); }
		
		git_struct_type* get() { return data_; }
		git_struct_type const* get() const { return data_; }
	protected:
		git_struct_type* data_ = nullptr;
	};

	class GitInstance_Impl
	{
	public:
		GitInstance_Impl(std::string name, std::string email);
		~GitInstance_Impl();

		std::string const& getName();
		std::string const& getEmail();
	private:
		std::string name_;
		std::string email_;
	};

	using GitInstance = std::shared_ptr < GitInstance_Impl >;

	// TODO: make this a pimpl
	class GitRepository
	{
	public:
		GitRepository(GitInstance instance, std::filesystem::path const& path);

		// All of these functions return true if sucessful
		bool addFile(std::filesystem::path const& file);
		bool addAllFiles();
		bool stageFile(std::filesystem::path const& file);
		bool stageAllFiles();
		bool commit(std::string const& commit_message);
		
		bool commitEverything(std::string const& commit_message);
	private:
		GitInstance instance_;
		std::filesystem::path path_;
	};

}
