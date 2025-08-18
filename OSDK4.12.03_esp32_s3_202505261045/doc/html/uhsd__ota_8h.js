var uhsd__ota_8h =
[
    [ "uhsd_ota_evt_func_cb", "group__grp__uhsdfota.html#ga2cd17ba27c5586e70f692b522b8814ed", null ],
    [ "uhsd_ota_online_whole_base_info_t", "group__grp__uhsdfota.html#gac8637fda45ffe546e7747e22f5324b9d", null ],
    [ "UHSD_OTA_ERROR_E", "group__grp__uhsdfota.html#gad3414abad484d5592bb8a618c47d5e24", [
      [ "UHSD_OTA_ERROR_OK", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24aac58ee6add1ee85d52dce69627d42b5b", null ],
      [ "UHSD_OTA_ERROR_OTHER", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24aaf594a561149edc5e6641163d06ffbb6", null ],
      [ "UHSD_OTA_ERROR_CONTENT_VALIDATION_FAILURE", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a7e19b9dc81a62290efbecd4808af1d30", null ],
      [ "UHSD_OTA_ERROR_ILLEGAL_CONTENT", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a70ded6ffb5f719020b362da13a3f4323", null ],
      [ "UHSD_OTA_ERROR_ENTITY_TOO_LARGE", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a57800ad4b43e7544d03d054c77878111", null ],
      [ "UHSD_OTA_ERROR_RESOURCE_NOT_AVAILABLE", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a6b981c00d7ddfa8140d6d2ff3329cef3", null ],
      [ "UHSD_OTA_ERROR_INCOMPLETE_CONTENT", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a389945034fccb4839682853153558e8a", null ],
      [ "UHSD_OTA_ERROR_DEVICE_BASEBOARD_UPGRADING", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a618ca9d60384e280a007411280b568f1", null ],
      [ "UHSD_OTA_ERROR_DEVICE_BASEBORAD_NOT_IN_SERVICE", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24aacb0d29694d74a2fcc87da39a0753696", null ],
      [ "UHSD_OTA_ERROR_DEVICE_MODULE_UPGRADING", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24ae24d6bae2b63cee00cb83a60233246b8", null ],
      [ "UHSD_OTA_ERROR_INVALID_UPGRADE_SN", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a87a8e714c0858d52dcd55121569452fb", null ],
      [ "UHSD_OTA_ERROR_BOUNDARY", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a7dd2c24d481de5a5d3f6f943161073c1", null ],
      [ "UHSD_OTA_ERROR_CURRENT_UPGRADING", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24ab1b48e6bb7af75ea07a96579dbc12a89", null ],
      [ "UHSD_OTA_ERROR_FRAGMENT_VALIDATION_FAILURE", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24af9b7f69d09b4a95eedb72dbfe8611975", null ],
      [ "UHSD_OTA_ERROR_BASEBOARD_FILE_SIZE", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24ae6ec7077b0f983dcfd301abddac0d6fe", null ],
      [ "UHSD_OTA_ERROR_DIGEST_MISMATCH", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24ad3f7fedb2b5c7a05b28fb5651292610c", null ],
      [ "UHSD_OTA_ERROR_UPDATE_TIMESUP", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a2e318060e0ddfe80d86bb2de457e3509", null ],
      [ "UHSD_OTA_ERROR_BASEBOARD_REJECT", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a3ed0b40bdef9e603e582fb58f962c6ad", null ],
      [ "UHSD_OTA_ERROR_BASEBOARD_FAIL", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24aec747a8252d07a6c97cfdbeeb0d05c69", null ],
      [ "UHSD_OTA_ERROR_BLE_CONFIGING", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a87f041d67bb74a316b57cc08ba3ef9ee", null ],
      [ "UHSD_OTA_ERROR_DEVICE_RESOURCE_INSUFFICIENT", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24aa978f5da6052f75529b4793562a4ce82", null ],
      [ "UHSD_OTA_ERROR_DEVICE_UPGRADE_TIMEOUT", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24afa76c97d8d80a704b9b465a9ec37e9ac", null ],
      [ "UHSD_OTA_ERROR_DEVICE_LOW_BATTERY", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a4fc3d9b2614775c06102fcd7b648b5ed", null ],
      [ "UHSD_OTA_ERROR_INTERNAL_UPGRADE_VER_SAME", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24ac2f9a87a1d1052af91e25e9a83eb277f", null ],
      [ "UHSD_OTA_ERROR_INTERNAL_UPGRADE_VER_TOO_LOW", "group__grp__uhsdfota.html#ggad3414abad484d5592bb8a618c47d5e24a1213dbc9c63385d1fb266df4f502b1b0", null ]
    ] ],
    [ "UHSD_OTA_EVT_E", "group__grp__uhsdfota.html#ga62ebe91278000bb7beee52d0cf367cce", [
      [ "UHSD_OTA_EVT_REQ", "group__grp__uhsdfota.html#gga62ebe91278000bb7beee52d0cf367ccea72399ca41307a524294c4032cfca63fb", null ],
      [ "UHSD_OTA_EVT_FIRM_READY", "group__grp__uhsdfota.html#gga62ebe91278000bb7beee52d0cf367ccea3bd1ca4204c19a6d9ca3a2b38801c68c", null ],
      [ "UHSD_OTA_EVT_SECTION_FIRM_DOWNLOAD_FINISH", "group__grp__uhsdfota.html#gga62ebe91278000bb7beee52d0cf367ccea46fd904137f3957d73972663749e19cb", null ],
      [ "UHSD_OTA_EVT_UPDATE_NEED_CONFIRM_FIRM", "group__grp__uhsdfota.html#gga62ebe91278000bb7beee52d0cf367ccea5f90750d9f4b6a39d15f70bd1d6148bc", null ],
      [ "UHSD_OTA_EVT_CONFIRM_FIRM_UPDATE_VALIID", "group__grp__uhsdfota.html#gga62ebe91278000bb7beee52d0cf367ccea6099429797288f34e891f9c536568cf1", null ],
      [ "UHSD_OTA_EVT_EXCEPTION_OCCURED", "group__grp__uhsdfota.html#gga62ebe91278000bb7beee52d0cf367ccea99f8d846f8fc431d9c0831cb97945879", null ]
    ] ],
    [ "UHSD_OTA_PKG_SRC_E", "group__grp__uhsdfota.html#gaced32dcd130535d859b824a7adc63505", [
      [ "UHSD_OTA_PKG_LOCAL", "group__grp__uhsdfota.html#ggaced32dcd130535d859b824a7adc63505a06365e4827330a793be05c2b4c404fd0", null ],
      [ "UHSD_OTA_PKG_ONLINE", "group__grp__uhsdfota.html#ggaced32dcd130535d859b824a7adc63505a6de9070130afe0bfb002280a0f814977", null ],
      [ "UHSD_OTA_PKG_MAX", "group__grp__uhsdfota.html#ggaced32dcd130535d859b824a7adc63505a31a1b1aa0054d258ba00e81117121a00", null ]
    ] ],
    [ "UHSD_OTA_REQ_RESULT_E", "group__grp__uhsdfota.html#ga112d41f6fec5bb519e1205f6ea1b0cd7", [
      [ "UHSD_OTA_REQ_RESULT_NOT_ALLOW", "group__grp__uhsdfota.html#gga112d41f6fec5bb519e1205f6ea1b0cd7ab0871295502452575c9a3127210186b4", null ],
      [ "UHSD_OTA_REQ_RESULT_LATER", "group__grp__uhsdfota.html#gga112d41f6fec5bb519e1205f6ea1b0cd7aa6a3374914f3280b2c089852ba145e75", null ],
      [ "UHSD_OTA_REQ_RESULT_ALLOW", "group__grp__uhsdfota.html#gga112d41f6fec5bb519e1205f6ea1b0cd7aadcd7868d33981eab1f7dd15686f3e68", null ]
    ] ],
    [ "UHSD_OTA_UPGRADE_DST_E", "group__grp__uhsdfota.html#gaf6524ed12e57736caac0a20ff7bc20fa", [
      [ "UHSD_OTA_UPGRADE_DST_WHOLE", "group__grp__uhsdfota.html#ggaf6524ed12e57736caac0a20ff7bc20faac3731db17607c6ae6e85bd3c44f1c2a2", null ],
      [ "UHSD_OTA_UPGRADE_DST_WIFI_MODULE", "group__grp__uhsdfota.html#ggaf6524ed12e57736caac0a20ff7bc20faa6eab9752265bf95eb160b427b316710c", null ],
      [ "UHSD_OTA_UPGRADE_DST_VOICE", "group__grp__uhsdfota.html#ggaf6524ed12e57736caac0a20ff7bc20faabf9fbb86936b0fd330595b97a7d0cdac", null ]
    ] ],
    [ "UHSD_OTA_UPGRADE_STATUS_E", "group__grp__uhsdfota.html#ga7beee17c9b1e741e7dcb4e9ce4e6bd4c", [
      [ "UHSD_OTA_UPGRADE_STATUS_IDLE_NO_DOWNLOAD", "group__grp__uhsdfota.html#gga7beee17c9b1e741e7dcb4e9ce4e6bd4ca1c4eb1b733f9347f21ea3346de92af64", null ],
      [ "UHSD_OTA_UPGRADE_STATUS_IDLE_DOWNLOADING", "group__grp__uhsdfota.html#gga7beee17c9b1e741e7dcb4e9ce4e6bd4cae432a00b2a639c54ed9f0e80f61a3c0f", null ],
      [ "UHSD_OTA_UPGRADE_STATUS_ALLOW", "group__grp__uhsdfota.html#gga7beee17c9b1e741e7dcb4e9ce4e6bd4ca6f2ab899e2ca1b31c68ce0e09399a14b", null ]
    ] ],
    [ "uhsd_ota_download_section_firm", "group__grp__uhsdfota.html#gad781326371528591a06259ccbda73112", null ],
    [ "uhsd_ota_evt_register", "group__grp__uhsdfota.html#ga123b7631142cfaa8a4a30f533ac8627a", null ],
    [ "uhsd_ota_get_online_sub_firm_info", "group__grp__uhsdfota.html#gaea30c00c19d3fe5910642323e9e0a695", null ],
    [ "uhsd_ota_get_online_whole_base_info", "group__grp__uhsdfota.html#ga09220012cb6ba200dce2a922d5cd805d", null ],
    [ "uhsd_ota_get_online_whole_desc_info", "group__grp__uhsdfota.html#ga4499987ceafe09e91b99c36ab9bc57b6", null ],
    [ "uhsd_ota_notify_req_result", "group__grp__uhsdfota.html#ga53edfdbafdcbc68b6ce0fade34bcb9bb", null ],
    [ "uhsd_ota_notify_start", "group__grp__uhsdfota.html#ga575f0e53b5b59e8edb82896dcc5b8dae", null ],
    [ "uhsd_ota_notify_stop", "group__grp__uhsdfota.html#ga18512ceecb4a2d6c207a626f04423dff", null ],
    [ "uhsd_ota_operation_set", "group__grp__uhsdfota.html#ga12aff091947a0e1dedd0e4a49fce35b1", null ],
    [ "uhsd_ota_upgrade_status_rpt", "group__grp__uhsdfota.html#ga107f7a8d0211c5484b4560a42c76edb0", null ],
    [ "uhsd_ota_version_qry", "group__grp__uhsdfota.html#ga2ca0d365e2643c32faec734d8f368ba9", null ],
    [ "uhsd_set_ota_download_auto_update", "group__grp__uhsdfota.html#ga71c8a2e0479252e0441a9bfce8bc2e8f", null ],
    [ "uhsd_set_ota_download_progress_notify_cb", "group__grp__uhsdfota.html#gac1383407401dd2715b4c793b6a9c9224", null ]
];