#pragma once

// PINGEQUA Scout Lite product labels.
// This header is the only place new brand strings may live.
// Do not include it from info, gpsdata, HARDWARE_NAME, SD update metadata,
// the boot banner, protocolinfo, or the WiGLE CSV header.
// The commit that adds this file does not include it from any translation unit.
// Upstream license stays MIT, Copyright (c) 2020 Just Call Me Koko (LICENSE).

namespace PingequaIdentity {

constexpr char kBrand[] = "PINGEQUA";
constexpr char kProduct[] = "Scout Lite";
constexpr char kFirmwareDisplayName[] = "PINGEQUA Scout Lite Firmware";
constexpr char kEdition[] = "Marauder Edition";
constexpr char kBoardId[] = "pingequa_scout_lite_c5";
constexpr char kHardwareDisplayName[] = "Scout Lite (PINGEQUA)";
constexpr char kFirmwareFamily[] = "Marauder";

// PINGEQUA layer SemVer. Not MARAUDER_VERSION. Not a release.
constexpr char kVersion[] = "0.1.0";
constexpr bool kVersionReleased = false;

// Future additive CLI. Not dispatched in the commit that adds this file.
// First token is exact-matched; these must not replace info or gpsdata.
constexpr char kCommandPrefix[] = "pq";
constexpr char kCommandInfo[] = "info";
constexpr char kCommandCaps[] = "caps";

// Reply labels. Values are the constants above, not a second copy of the brand.
// Lines must start with "PQ " so they are not Firmware:, Version:, or Hardware:.
constexpr char kReplyPrefix[] = "PQ ";
constexpr char kReplyBrand[] = "PQ Brand: ";
constexpr char kReplyProduct[] = "PQ Product: ";
constexpr char kReplyDisplay[] = "PQ Display: ";
constexpr char kReplyEdition[] = "PQ Edition: ";
constexpr char kReplyBoard[] = "PQ Board: ";
constexpr char kReplyHardware[] = "PQ Hardware: ";
constexpr char kReplyFamily[] = "PQ Family: ";
constexpr char kReplyVersion[] = "PQ Version: ";
constexpr char kReplyReleased[] = "PQ Released: ";
constexpr char kReplyMarauder[] = "PQ Marauder: ";
constexpr char kReplyCapsBegin[] = "PQ Caps-Begin";
constexpr char kReplyCapsEnd[] = "PQ Caps-End";
constexpr char kReplyCap[] = "PQ Cap: ";
constexpr char kReplyError[] = "PQ Error: unknown";
constexpr char kReplyReleasedNo[] = "no";

}  // namespace PingequaIdentity
