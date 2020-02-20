#pragma once
#include <cppgit2/exception.hpp>
#include <cppgit2/libgit2_api.hpp>
#include <cppgit2/object.hpp>
#include <cppgit2/ownership.hpp>
#include <git2.h>
#include <string>

namespace cppgit2 {

class reference : public libgit2_api {
public:
  reference();
  reference(git_reference *c_ptr = nullptr,
            ownership owner = ownership::libgit2);
  ~reference();

  // Basic type of any Git reference
  enum class reference_type {
    invalid = 0,  // Invalid reference
    direct = 1,   // A reference that points at an object id
    symbolic = 2, // A reference that points at another reference
    all = direct | symbolic
  };

  // Compare two references.
  // 0 if the same, else a stable but meaningless ordering.
  int compare(const reference &rhs) const;

  // Delete an existing reference.
  static void delete_reference(reference &ref);

  // Create a duplicate of an existing reference
  reference copy() const;

  // Check if this reference ...
  bool is_branch() const;
  bool is_note() const;
  bool is_remote() const;
  bool is_tag() const;

  // Check if a reference name is well-formed.
  // Valid reference names must follow one of two patterns:
  // 1. Top-level names must contain only capital letters and underscores, and
  // must begin and end
  //    with a letter. (e.g. "HEAD", "ORIG_HEAD").
  // 2. Names prefixed with "refs/" can be almost
  //    anything. You must avoid the characters '~', '^', ':', '\', '?', '[',
  //    and '*', and the sequences ".." and "@{" which have special meaning to
  //    revparse.
  static bool is_valid_name(const std::string &refname);

  // Full name of this reference
  std::string name() const;

  // This will transform the reference name into a name "human-readable"
  // version. If no shortname is appropriate, it will return the full name.
  std::string shorthand_name() const;

  // Get the repository where a reference resides
  class repository owner() const;

  // Recursively peel reference until object of the specified type is found.
  object peel_until(object::object_type type);

  // Resolve a sym reference to a direct reference
  // If a direct reference is passed as an argument, a copy of that reference is
  // returned. This copy must be manually freed too.
  reference resolve();

  // Get the OID pointed by a direct reference
  // Only available if the reference is direct (i.e. an object id reference, not
  // a symbolic one).
  //
  // To find the OID of a symbolic ref, call reference.resolve() and then this
  // function (or maybe use reference.name_to_id() to directly resolve a
  // reference name all the way through to an OID).
  oid target() const;

  // Return the peeled OID target of this reference
  oid peeled_target() const;

  // Get the type of a reference
  reference_type type() const;

  // Access libgit2 C ptr
  const git_reference *c_ptr() const;

private:
  git_reference *c_ptr_;
  ownership owner_;
};

} // namespace cppgit2