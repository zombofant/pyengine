/**********************************************************************
File name: Utils.cpp
This file is part of: Pythonic Engine

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about pyengine please e-mail one of the
authors named in the AUTHORS file.
**********************************************************************/
#include <catch.hpp>

#include <CEngine/VFS/Utils.hpp>

using namespace PyEngine;

TEST_CASE("VFS/Utils/absolutify/keeps_absolute_paths",
          "absolutify must not change absolute paths")
{
    std::string path = "/some/test/path";
    CHECK(path == absolutify(path));
}

TEST_CASE("VFS/Utils/absolutify/dot",
          "absolutify must remove single `.' elements")
{
    std::string path_abs = "/some/test/path";
    std::string path_rel = "/./some/./test/./path";
    CHECK(path_abs == absolutify(path_rel));
}

TEST_CASE("VFS/Utils/absolutify/ddot",
          "absolutify must properly deal with `..' elements")
{
    std::string path_abs = "/some/other/test/path";
    std::string path_rel = "/some/test/../other/test/../test/path";
    CHECK(path_abs == absolutify(path_rel));
}

TEST_CASE("VFS/Utils/absolutify/single_file",
          "absolutify must absolutify a single file")
{
    std::string path = "test.txt";
    std::string path_norm = "/test.txt";
    CHECK(path_norm == absolutify(path));
}

TEST_CASE("VFS/Utils/basename/only_basename",
          "basename must find the basename of a file without path")
{
    std::string path = "test.txt";
    std::string basenamed = "test.txt";
    CHECK(basenamed == basename(path));
}

TEST_CASE("VFS/Utils/basename/long_path",
          "basename must find the basename of a usual long path")
{
    std::string path = "/foo/bar/baz/test.txt";
    std::string basenamed = "test.txt";
    CHECK(basenamed == basename(path));
}

TEST_CASE("VFS/Utils/dirname/only_basename",
          "dirname must return empty result for a file without path")
{
    std::string path = "test.txt";
    std::string dirnamed = "";
    CHECK(dirnamed == dirname(path));
}

TEST_CASE("VFS/Utils/dirname/long_path",
          "dirname must find the dirname of a usual long path")
{
    std::string path = "/foo/bar/baz/test.txt";
    std::string dirnamed = "/foo/bar/baz";
    CHECK(dirnamed == dirname(path));
}

TEST_CASE("VFS/Utils/normalize_vfs_path/keeps_normalized_paths",
          "normalize_vfs_path must keep absolute paths")
{
    std::string path = "/some/normalized/path";
    CHECK(path == normalize_vfs_path(path));
}

TEST_CASE("VFS/Utils/normalize_vfs_path/remove_trailing",
          "normalize_vfs_path must remove trailing slashes")
{
    std::string path = "/with/trailing/path/";
    std::string path_norm = "/with/trailing/path";
    CHECK(path_norm == normalize_vfs_path(path));
}

TEST_CASE("VFS/Utils/join/one_root",
          "join must properly join paths with only one absolute path")
{
    CHECK("/some/test/path" == join({"/some/test", "path"}));
    CHECK("/some/longer/test/path" == join({"/some/longer", "test", "path"}));
    CHECK("keeps/non/trailing/slashes" == join({"keeps", "non", "trailing", "slashes"}));
}

TEST_CASE("VFS/Utils/join/multiple_roots",
          "join must properly join paths with multiple absolute paths")
{
    CHECK("/some/test/path" == join({"garbage/path", "/some", "test", "path"}));

}

TEST_CASE("VFS/Utils/splitext/simple",
          "simple cases for split ext")
{
    std::pair<std::string, std::string> val = std::make_pair(
        "/root/path/test", "txt");
    CHECK(val == splitext("/root/path/test.txt"));

    val = std::make_pair("root/path/test", "txt");
    CHECK(val == splitext("root/path/test.txt"));
}

TEST_CASE("VFS/Utils/splitext/dot_in_path",
          "splitext must work properly if a dot is in the path")
{
    std::pair<std::string, std::string> val = std::make_pair(
        "/root/path.git/test", "txt");
    CHECK(val == splitext("/root/path.git/test.txt"));

    val = std::make_pair("root/path.git/test", "txt");
    CHECK(val == splitext("root/path.git/test.txt"));
}
