/*
 *  Copyright (c) 2017, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include <proxygen/lib/http/codec/compress/HeaderIndexingStrategy.h>

#include <folly/Singleton.h>

namespace {
using namespace proxygen;
folly::Singleton<HeaderIndexingStrategy> defaultHeaderIndexingStrategyInstance;
}

namespace proxygen {

std::shared_ptr<HeaderIndexingStrategy>
    HeaderIndexingStrategy::getDefaultInstance() {
  return defaultHeaderIndexingStrategyInstance.try_get();
}

bool HeaderIndexingStrategy::indexHeader(const HPACKHeader& header) const {
  // Handle all the cases where we want to return false in the switch statement
  // below; else let the code fall through and return true
  switch(header.name.getHeaderCode()) {
    case HTTP_HEADER_OTHER:
      if (header.name.get() == ":path") {
        if (header.value.find('=') != std::string::npos) {
          return false;
        }
        if (header.value.find("jpg") != std::string::npos) {
          return false;
        }
      }
      break;

    // The wrapped header should never be HTTP_HEADER_NONE but for completeness
    // the condition is included below
    case HTTP_HEADER_NONE:
    case HTTP_HEADER_CONTENT_LENGTH:
    case HTTP_HEADER_IF_MODIFIED_SINCE:
    case HTTP_HEADER_LAST_MODIFIED:
      return false;

    default:
      break;
  }

  return true;
}

}
