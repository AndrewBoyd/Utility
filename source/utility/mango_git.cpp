#include "mango_git.h"
#include <fmt/format.h>
#include <git2.h>
#include <iostream>
#include <functional>
#include <chrono>

namespace mango::git
{
	template <typename return_t, typename ... args_t> inline
		return_t makeGitCallback(args_t...args, void* payload)
	{
		using func_t = std::function<return_t(args_t...)>;
		auto func = *static_cast<func_t*>(payload);
		return func(args...);
	}

	template <typename return_t, typename ... args_t> inline
		void* makeGitCallbackPayload(std::function<return_t(const char*, const char*)> const& func)
	{
		return (void*)(&func);
	}

	int gitMatchedPathCallback(const char* path, const char* matched_pathspec, void* payload)
	{
		return makeGitCallback<int, const char*, const char*>(path, matched_pathspec, payload);
	}

	void* gitMatchedPathCallbackPayload(std::function<int(const char*, const char*)> const& func)
	{
		return makeGitCallbackPayload<int, const char*, const char*>(func);
	}


	///////////////////////////////////
	///////////////////////////////////

	git_time_t getCurrentTime()
	{
		using namespace std::chrono;
		auto const now = system_clock::now();
		auto const since_epoch = now.time_since_epoch();
		auto const as_seconds = duration_cast<seconds>(since_epoch);
		return as_seconds.count();
	}

	///////////////////////////////////
	///////////////////////////////////

	class Oid
	{
	public:
		Oid() {};

		Oid(const char* string) {
			git_oid_fromstr(get(), string);
		}

		Oid(std::string const& string)
			: Oid(string.c_str())
		{ }

		static Oid fromReferenceName(git_repository* repo, const char* name, bool* success = nullptr) {
			auto result = Oid{};
			auto error = git_reference_name_to_id(result.get(), repo, name);
			if (success) *success = !error;
			return result;
		}

		git_oid* get() { return &data_; }
		git_oid const* get() const { return &data_; }
	private:
		git_oid data_ = {};
	};

	class Signature : public GitType<git_signature, git_signature_free>
	{
	public:
		Signature(GitInstance instance)
			: GitType(git_signature_new,
				instance->getName().c_str(),
				instance->getEmail().c_str(),
				getCurrentTime(), 0)
		{}
	};

	class Repository : public GitType<git_repository, git_repository_free>
	{
	public:
		Repository(std::filesystem::path const& path_to_directory)
			: GitType(git_repository_open, std::filesystem::canonical(path_to_directory).string().c_str())
		{}
	};

	class Reference : public GitType<git_reference, git_reference_free>
	{
	public:
		using GitType::GitType;

		static Reference fromBranch(git_repository* repo, const char* str) {
			return Reference(git_reference_dwim, repo, str);
		}

		static Reference fromFullName(git_repository* repo, const char* str) {
			return Reference(git_reference_lookup, repo, str);
		}
	};

	class Commit : public GitType<git_commit, git_commit_free>
	{
	public:
		Commit(git_repository* repo, git_oid* id)
			: GitType(git_commit_lookup, repo, id)
		{ }
	};

	class Tree : public GitType<git_tree, git_tree_free>
	{
	public:
		Tree(git_commit* commit)
			: GitType(git_commit_tree, commit)
		{ }

		Tree(git_repository* repo, git_oid* oid)
			: GitType(git_tree_lookup, repo, oid)
		{ }
	};

	class Index : public GitType<git_index, git_index_free>
	{
	public:
		Index(git_repository* repo)
			: GitType(git_repository_index, repo)
		{ }
	};

	////////////////////////////////////////////
	////////////////////////////////////////////

	GitInstance_Impl::GitInstance_Impl(std::string name, std::string email)
		: name_(std::move(name))
		, email_(std::move(email))
	{
		git_libgit2_init();
	}

	GitInstance_Impl::~GitInstance_Impl()
	{
		git_libgit2_shutdown();
	}

	std::string const& GitInstance_Impl::getName()
	{
		return name_;
	}

	std::string const& GitInstance_Impl::getEmail()
	{
		return email_;
	}

	///////////////////////////////////////////
	///////////////////////////////////////////

	GitRepository::GitRepository(GitInstance instance, std::filesystem::path const& path)
		: instance_(instance)
		, path_(path)
	{
	}

	bool GitRepository::addFile(std::filesystem::path const& file)
	{
		return false;
	}

	bool GitRepository::addAllFiles()
	{
		return false;
	}

	bool GitRepository::stageFile(std::filesystem::path const& file)
	{
		return false;
	}

	bool GitRepository::stageAllFiles()
	{
		return false;
	}

	bool GitRepository::commit(std::string const& commit_message)
	{
		auto repo = Repository(path_);
		auto index = Index(repo.get());

		auto error = git_index_add_all(index.get(), { 0 }, 0, nullptr, nullptr);
		if (error != 0) return false;

		auto tree_oid = Oid();
		git_index_write_tree(tree_oid.get(), index.get());
		auto tree = Tree(repo.get(), tree_oid.get());

		auto signature = Signature(instance_);

		auto has_parent = false;
		auto parent_oid = Oid::fromReferenceName(repo.get(), "HEAD", &has_parent);
		auto parent = Commit(repo.get(), parent_oid.get());

		auto commit_oid = Oid();
		error = has_parent
			? git_commit_create_v(commit_oid.get(), repo.get(), "HEAD", signature.get(), signature.get(), NULL, commit_message.c_str(), tree.get(), 1, parent.get())
			: git_commit_create_v(commit_oid.get(), repo.get(), "HEAD", signature.get(), signature.get(), NULL, commit_message.c_str(), tree.get(), 0);
			

		git_index_write(index.get());

		return (error == 0);
	}

	bool GitRepository::commitEverything(std::string const& commit_message)
	{
		return false;
	}

}

