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
 * \brief CoreML runtime that can run coreml model
 *        containing only tvm PackedFunc.
 * \file coreml_runtime.h
 */
#ifndef TVM_RUNTIME_CONTRIB_VAI_VAI_RUNTIME_H_
#define TVM_RUNTIME_CONTRIB_VAI_VAI_RUNTIME_H_



#include <dlpack/dlpack.h>
#include <tvm/runtime/ndarray.h>
#include <tvm/runtime/packed_func.h>

#include <memory>
#include <string>
#include <vector>

#include<pyxir/pyxir.hpp>
#include <pyxir/runtime/run_options.hpp>
#include <fstream>



namespace tvm {
namespace runtime {

/*!
 * \brief CoreML runtime.
 *
 *  This runtime can be accessed in various language via
 *  TVM runtime PackedFunc API.
 */
class VaiRuntime : public ModuleNode {
 public:
 VaiRuntime(const std::string& symbol_name, const std::string& graph_json,
                  const Array<String> const_names):
          symbol_name_(symbol_name), graph_json_(graph_json), const_names_(const_names) {
            this->symbol_name_= symbol_name;
            this->const_names_ = { };

          }
          
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
  const char* type_key() const { return "VaiRuntime"; }

  /*!
   * \brief Initialize the coreml runtime with coreml model and context.
   * \param model_path The compiled model path.
   * \param ctx The context where the coreml model will be executed on.
   * \param output_names The output names of the model.
   */

 void Init(const std::string& model_path,const std::string& target, const std::vector<std::string> out_tensor_names);
 //void Init(const std::string& model_path,const std::string& target, const std::string out_tensor_names_);

  /*!
   * \brief Serialize the content of the mlmodelc directory and save it to
   *        binary stream.
   * \param stream The binary stream to save to.
   */
  void SaveToBinary(dmlc::Stream* stream) final;
  /*!
   * \brief set input to the model.
   * \param key The input name.
   * \param data_in The input data.
   */
  //void SetInput(const std::string& key, DLTensor* data_in);
  /*!
   * \brief Return NDArray for given output index.
   * \param index The output index.
   *
   * \return NDArray corresponding to given output node index.
   */
  //NDArray GetOutput(int index) const;
  /*!
   * \brief Return the number of outputs
   *
   * \return The number of outputs
   */
  //int GetNumOutputs() const;

  // get xgraph model
  std::shared_ptr<pyxir::graph::XGraph> xgraph_;

  //RT module

   /*! \brief The only subgraph name for this module. */
  std::string symbol_name_;
  /*! \brief The graph. */
  std::string graph_json_;
  /*! \brief The required constant names. */
  Array<String> const_names_;

  pyxir::RtModHolder rt_mod_;
  std::string curr_subgraph_;
  std::string model_path_;
  std::string target_;
  //std::vector<std::string> target;
  std::vector<std::string> in_tensor_names_;
  std::vector<std::string> out_tensor_names_;
  bool initialized_{false};



  // TVM context
  TVMContext ctx_;
};

}  // namespace runtime
}  // namespace tvm




#endif  // TVM_RUNTIME_CONTRIB_VAI_VAI_RUNTIME_H_
