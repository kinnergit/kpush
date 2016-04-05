--config = {
--    pem_file = "./test.pem",
--    pass_phrase = "weiboyi",
----    apns_push_host = "gateway.sandbox.push.apple.com",
--    apns_push_host = "17.172.232.18",
--    test_devtoken = "84e2b4d44912a517d906ab9d7c4c246ec8bf812a1c4d674e3de82e80bc8e36da",
--    test_msg = "%d冯,Are you crazy?",
--    apns_push_port = 2195,
--    apns_connect_timeout = 15,
--    apns_batch_send_count = 32,
--    apns_batch_buffer = 8192,
--    apns_send_total = 1,
--};


config = {
    pem_file = "./cert.pem",
    pass_phrase = "weiboyi",
    apns_push_host = "17.110.226.216",
    test_devtoken = "039dc8a47e8ded811adf40788bda2ca0d7b933115795f4835f433a6116f18739",
    test_msg = "%dMessChapters Gone!",
    apns_push_port = 2195,
    apns_connect_timeout = 15,
    apns_batch_send_count = 32,
    apns_batch_buffer = 8192,
    apns_send_total = 200,
};

return config;