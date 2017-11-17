// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Buff.proto

#ifndef PROTOBUF_Buff_2eproto__INCLUDED
#define PROTOBUF_Buff_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "CommomDef.pb.h"
// @@protoc_insertion_point(includes)

namespace pb {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Buff_2eproto();
void protobuf_AssignDesc_Buff_2eproto();
void protobuf_ShutdownFile_Buff_2eproto();

class GS2C_Buff_All_Info;

// ===================================================================

class GS2C_Buff_All_Info : public ::google::protobuf::Message {
 public:
  GS2C_Buff_All_Info();
  virtual ~GS2C_Buff_All_Info();

  GS2C_Buff_All_Info(const GS2C_Buff_All_Info& from);

  inline GS2C_Buff_All_Info& operator=(const GS2C_Buff_All_Info& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const GS2C_Buff_All_Info& default_instance();

  void Swap(GS2C_Buff_All_Info* other);

  // implements Message ----------------------------------------------

  GS2C_Buff_All_Info* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GS2C_Buff_All_Info& from);
  void MergeFrom(const GS2C_Buff_All_Info& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .pb.PairTime buff = 1;
  inline int buff_size() const;
  inline void clear_buff();
  static const int kBuffFieldNumber = 1;
  inline const ::pb::PairTime& buff(int index) const;
  inline ::pb::PairTime* mutable_buff(int index);
  inline ::pb::PairTime* add_buff();
  inline const ::google::protobuf::RepeatedPtrField< ::pb::PairTime >&
      buff() const;
  inline ::google::protobuf::RepeatedPtrField< ::pb::PairTime >*
      mutable_buff();

  // @@protoc_insertion_point(class_scope:pb.GS2C_Buff_All_Info)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::pb::PairTime > buff_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_Buff_2eproto();
  friend void protobuf_AssignDesc_Buff_2eproto();
  friend void protobuf_ShutdownFile_Buff_2eproto();

  void InitAsDefaultInstance();
  static GS2C_Buff_All_Info* default_instance_;
};
// ===================================================================


// ===================================================================

// GS2C_Buff_All_Info

// repeated .pb.PairTime buff = 1;
inline int GS2C_Buff_All_Info::buff_size() const {
  return buff_.size();
}
inline void GS2C_Buff_All_Info::clear_buff() {
  buff_.Clear();
}
inline const ::pb::PairTime& GS2C_Buff_All_Info::buff(int index) const {
  return buff_.Get(index);
}
inline ::pb::PairTime* GS2C_Buff_All_Info::mutable_buff(int index) {
  return buff_.Mutable(index);
}
inline ::pb::PairTime* GS2C_Buff_All_Info::add_buff() {
  return buff_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::pb::PairTime >&
GS2C_Buff_All_Info::buff() const {
  return buff_;
}
inline ::google::protobuf::RepeatedPtrField< ::pb::PairTime >*
GS2C_Buff_All_Info::mutable_buff() {
  return &buff_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Buff_2eproto__INCLUDED