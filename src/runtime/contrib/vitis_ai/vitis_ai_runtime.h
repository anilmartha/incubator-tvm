/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*!
 * \brief Vitis-AI runtime that can run model
 *        containing only tvm PackedFunc.
 * \file vitis_ai_runtime.h
 */
#ifndef TVM_RUNTIME_CONTRIB_VITIS_AI_RUNTIME_H_
#define TVM_RUNTIME_CONTRIB_VITIS_AI_RUNTIME_H_


#include <dlpack/dlpack.h>
#include <tvm/runtime/ndarray.h>
#include <tvm/runtime/packed_func.h>

#include <memory>
#include <string>
#include <vector>

#include<pyxir/pyxir.hpp>
#include <pyxir/runtime/run_options.hpp>



namespace tvm {
namespace runtime {

/*!
 * \brief VAI runtime.
 *
 *  This runtime can be accessed in various language via
 *  TVM runtime PackedFunc API.
 */
class VitisAIRuntime : public ModuleNode {
 public:
VitisAIRuntime(const std::string& symbol_name, const Array<String> const_names):
  symbol_name_(symbol_name), const_names_(const_names) {}
  /*!
   * \brief Get member function to front-end.
   * \param name The name of the function.
   * \param sptr_to_self The pointer to the module node.
   * \return The corresponding member function.
   */
  virtual PackedFunc GetFunction(const std::string& name, const ObjectPtr<Object>& sptr_to_self);
  /*!
   * \return The type key of the executor.
   */
  const char* type_key() const { return "VitisAIRuntime"; }

  /*!
   * \brief Initialize the vai runtime with pyxir.
   * \param model_path The compiled model path.
   * \param target The name of the target being used
   */
void Init(const std::string& model_path, const std::string& target);
  /*!
   * \brief Serialize the content of the pyxir directory and save it to
   *        binary stream.
   * \param stream The binary stream to save to.
   */
  void SaveToBinary(dmlc::Stream* stream) final;

 private:
  /*! \brief The only subgraph name for this module. */
  std::string symbol_name_;
  /*! \brief The required constant names. */
  Array<String> const_names_;
  std::shared_ptr<pyxir::graph::XGraph> xgraph_;
  pyxir::RtModHolder rt_mod_;
  std::string model_path_;
  std::string target_;
  std::vector<std::string> in_tensor_names_;
  std::vector<std::string> out_tensor_names_;
  bool initialized_{false};
};
}  // namespace runtime
}  // namespace tvm
#endif  // TVM_RUNTIME_CONTRIB_VITIS_AI_RUNTIME_H_