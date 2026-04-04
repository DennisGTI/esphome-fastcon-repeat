void FastconLight::write_state(light::LightState *state) {
  auto light_data = this->controller_->get_light_data(state);

  bool is_on = (light_data[0] & 0x80) != 0;
  float brightness = ((light_data[0] & 0x7F) / 127.0f) * 100.0f;

  if (light_data.size() == 1) {
    ESP_LOGD(TAG, "Writing state: light_id=%d, on=%d, brightness=%.1f%%", light_id_, is_on, brightness);
  } else {
    auto r = light_data[2];
    auto g = light_data[3];
    auto b = light_data[1];
    auto warm = light_data[4];
    auto cold = light_data[5];
    ESP_LOGD(TAG, "Writing state: light_id=%d, on=%d, brightness=%.1f%%, rgb=(%d,%d,%d), warm=%d, cold=%d",
             light_id_, is_on, brightness, r, g, b, warm, cold);
  }

  auto adv_data = this->controller_->single_control(this->light_id_, light_data);

  auto hex_str = vector_to_hex_string(adv_data).data();
  ESP_LOGD(TAG, "Advertisement Payload (%d bytes): %s", adv_data.size(), hex_str);

  for (uint8_t i = 0; i < this->controller_->get_repeat_count(); i++) {
    this->controller_->queueCommand(this->light_id_, adv_data);
  }
}
