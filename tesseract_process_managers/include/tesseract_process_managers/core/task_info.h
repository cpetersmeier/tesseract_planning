/**
 * @file task_info.h
 * @brief Process Info
 *
 * @author Matthew Powelson
 * @date July 15. 2020
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2020, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TESSERACT_PROCESS_MANAGERS_task_info_H
#define TESSERACT_PROCESS_MANAGERS_task_info_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <memory>
#include <shared_mutex>
#include <map>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_command_language/core/instruction.h>
#include <tesseract_command_language/null_instruction.h>
#include <tesseract_environment/environment.h>

#ifdef SWIG
%wrap_unique_ptr(TaskInfoUPtr,tesseract_planning::TaskInfo)
%shared_ptr(tesseract_planning::TaskInfoContainer)
#endif  // SWIG

namespace tesseract_planning
{
/** Stores information about a Task */
class TaskInfo
{
public:
  using Ptr = std::shared_ptr<TaskInfo>;
  using ConstPtr = std::shared_ptr<const TaskInfo>;
  using UPtr = std::unique_ptr<TaskInfo>;
  using ConstUPtr = std::unique_ptr<const TaskInfo>;

  TaskInfo() = default;  // Required for serialization
  TaskInfo(std::size_t unique_id, std::string name = "");
  virtual ~TaskInfo() = default;
  TaskInfo(const TaskInfo&) = default;
  TaskInfo& operator=(const TaskInfo&) = default;
  TaskInfo(TaskInfo&&) = default;
  TaskInfo& operator=(TaskInfo&&) = default;

  /** @brief Value returned from the Task on completion */
  int return_value{ std::numeric_limits<int>::lowest() };

  /** @brief Unique ID generated for the Task by Taskflow */
  std::size_t unique_id{ 0 };

  std::string task_name;

  std::string message;

  /** @brief elapsed_time Time spent in this task in seconds*/
  double elapsed_time{ 0 };

  /** @brief Instructions passed to task (optionally set) */
  Instruction instructions_input{ NullInstruction() };
  /** @brief Instructions after running the task (optionally set)*/
  Instruction instructions_output{ NullInstruction() };
  /** @brief Seed/Results passed into the task (optionally set) */
  Instruction results_input{ NullInstruction() };
  /** @brief Seed/Results after running the task (optionally set)*/
  Instruction results_output{ NullInstruction() };
  /** @brief The environment at the beginning of the task (optionally set)*/
  tesseract_environment::Environment::ConstPtr environment{ nullptr };

  bool operator==(const TaskInfo& rhs) const;
  bool operator!=(const TaskInfo& rhs) const;

  virtual TaskInfo::UPtr clone() const;

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};

/** @brief A threadsafe container for TaskInfos */
struct TaskInfoContainer
{
  using Ptr = std::shared_ptr<TaskInfoContainer>;
  using ConstPtr = std::shared_ptr<const TaskInfoContainer>;

  void addTaskInfo(TaskInfo::UPtr task_info);

  TaskInfo::UPtr operator[](std::size_t index) const;

  /** @brief Get a copy of the task_info_map_ in case it gets resized*/
  std::map<std::size_t, TaskInfo::UPtr> getTaskInfoMap() const;

  bool operator==(const TaskInfoContainer& rhs) const;
  bool operator!=(const TaskInfoContainer& rhs) const;

private:
  mutable std::shared_mutex mutex_;
  std::map<std::size_t, TaskInfo::UPtr> task_info_map_;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};
}  // namespace tesseract_planning

BOOST_CLASS_EXPORT_KEY2(tesseract_planning::TaskInfo, "TaskInfo")
#endif  // TESSERACT_PROCESS_MANAGERS_task_info_H
