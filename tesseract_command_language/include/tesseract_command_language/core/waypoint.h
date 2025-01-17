/**
 * @file waypoint.h
 * @brief
 *
 * @author Levi Armstrong
 * @date June 15, 2020
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
#ifndef TESSERACT_COMMAND_LANGUAGE_WAYPOINT_H
#define TESSERACT_COMMAND_LANGUAGE_WAYPOINT_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <memory>
#include <string>
#include <typeindex>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/concept_check.hpp>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_common/serialization.h>
#include <tesseract_common/type_erasure.h>

#ifdef SWIG
//%template(Waypoints) std::vector<tesseract_planning::Waypoint>;
%ignore tesseract_planning::Waypoint::getType;
#endif  // SWIG

/** @brief If shared library, this must go in the header after the class definition */
#define TESSERACT_WAYPOINT_EXPORT_KEY(N, C)                                                                            \
  namespace N                                                                                                          \
  {                                                                                                                    \
  using C##InstanceBase =                                                                                              \
      tesseract_common::TypeErasureInstance<C, tesseract_planning::detail_waypoint::WaypointInterface>;                \
  using C##Instance = tesseract_planning::detail_waypoint::WaypointInstance<C>;                                        \
  using C##InstanceWrapper = tesseract_common::TypeErasureInstanceWrapper<C##Instance>;                                \
  }                                                                                                                    \
  BOOST_CLASS_EXPORT_KEY(N::C##InstanceBase)                                                                           \
  BOOST_CLASS_EXPORT_KEY(N::C##Instance)                                                                               \
  BOOST_CLASS_EXPORT_KEY(N::C##InstanceWrapper)                                                                        \
  BOOST_CLASS_TRACKING(N::C##InstanceBase, boost::serialization::track_never)                                          \
  BOOST_CLASS_TRACKING(N::C##Instance, boost::serialization::track_never)                                              \
  BOOST_CLASS_TRACKING(N::C##InstanceWrapper, boost::serialization::track_never)

/** @brief If shared library, this must go in the cpp after the implicit instantiation of the serialize function */
#define TESSERACT_WAYPOINT_EXPORT_IMPLEMENT(inst)                                                                      \
  BOOST_CLASS_EXPORT_IMPLEMENT(inst##InstanceBase)                                                                     \
  BOOST_CLASS_EXPORT_IMPLEMENT(inst##Instance)                                                                         \
  BOOST_CLASS_EXPORT_IMPLEMENT(inst##InstanceWrapper)

/**
 * @brief This should not be used within shared libraries use the two above.
 * If not in a shared library it can go in header or cpp
 */
#define TESSERACT_WAYPOINT_EXPORT(N, C)                                                                                \
  TESSERACT_WAYPOINT_EXPORT_KEY(N, C)                                                                                  \
  TESSERACT_WAYPOINT_EXPORT_IMPLEMENT(N::C)

namespace tesseract_planning
{
#ifndef SWIG
namespace detail_waypoint
{
template <typename T>
struct WaypointConcept  // NOLINT
  : boost::Assignable<T>,
    boost::CopyConstructible<T>,
    boost::EqualityComparable<T>
{
  BOOST_CONCEPT_USAGE(WaypointConcept)
  {
    T cp(c);
    T assign = c;
    bool eq = (c == cp);
    bool neq = (c != cp);
    UNUSED(assign);
    UNUSED(eq);
    UNUSED(neq);

    c.print();
    c.print("prefix_");
  }

private:
  T c;
};

struct WaypointInterface : tesseract_common::TypeErasureInterface
{
  virtual void print(const std::string& prefix) const = 0;

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};

template <typename T>
struct WaypointInstance : tesseract_common::TypeErasureInstance<T, WaypointInterface>  // NOLINT
{
  using BaseType = tesseract_common::TypeErasureInstance<T, WaypointInterface>;
  WaypointInstance() = default;
  WaypointInstance(const T& x) : BaseType(x) {}
  WaypointInstance(WaypointInstance&& x) noexcept : BaseType(std::move(x)) {}

  BOOST_CONCEPT_ASSERT((WaypointConcept<T>));

  void print(const std::string& prefix) const final { this->get().print(prefix); }

private:
  friend class boost::serialization::access;
  friend struct tesseract_common::Serialization;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)  // NOLINT
  {
    ar& boost::serialization::make_nvp("base", boost::serialization::base_object<BaseType>(*this));
  }
};
}  // namespace detail_waypoint
#endif  // SWIG
}  // namespace tesseract_planning

namespace tesseract_planning
{
using WaypointBase =
    tesseract_common::TypeErasureBase<detail_waypoint::WaypointInterface, detail_waypoint::WaypointInstance>;
struct Waypoint : WaypointBase
{
  using WaypointBase::WaypointBase;

  void print(const std::string& prefix = "") const;

private:
  friend class boost::serialization::access;
  friend struct tesseract_common::Serialization;

  template <class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/);  // NOLINT
};

}  // namespace tesseract_planning

#ifndef SWIG
BOOST_SERIALIZATION_ASSUME_ABSTRACT(tesseract_planning::detail_waypoint::WaypointInterface)
BOOST_CLASS_EXPORT_KEY(tesseract_planning::detail_waypoint::WaypointInterface)
BOOST_CLASS_TRACKING(tesseract_planning::detail_waypoint::WaypointInterface, boost::serialization::track_never)

BOOST_CLASS_EXPORT_KEY(tesseract_planning::WaypointBase)
BOOST_CLASS_TRACKING(tesseract_planning::WaypointBase, boost::serialization::track_never)

BOOST_CLASS_EXPORT_KEY(tesseract_planning::Waypoint)
BOOST_CLASS_TRACKING(tesseract_planning::Waypoint, boost::serialization::track_never);
#endif  // SWIG

#endif  // TESSERACT_COMMAND_LANGUAGE_WAYPOINT_H
