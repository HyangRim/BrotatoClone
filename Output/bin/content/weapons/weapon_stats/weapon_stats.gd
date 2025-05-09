class_name WeaponStats
extends Resource

export(int) var cooldown := 60 # ticks - we have 60 ticks per second
export(int) var damage := 1
export(float, 0.0, 1.0, 0.05) var accuracy = 1.0
export(float, 0.0, 1.0, 0.01) var crit_chance = 0.03
export(float) var crit_damage = 1.5
export(int, 0, 10000) var min_range := 0
export(int, 0, 10000) var max_range := 150
export(int, -10000, 10000) var knockback := 0
export(float, 0.0, 1.0, 0.05) var knockback_piercing := 0.0 # Bypasses enemies' knockback_resistance stat
export(bool) var can_have_positive_knockback := true
export(bool) var can_have_negative_knockback := false
export(float, 0.0, 1.0, 0.1) var effect_scale := 1.0
export(Array, Array) var scaling_stats = [["stat_melee_damage", 1.0]]
export(float, 0, 1.0, 0.01) var lifesteal := 0.0
export(Array, Resource) var shooting_sounds
export(int) var sound_db_mod = -5
export(bool) var is_healing = false
export(Resource) var custom_on_cooldown_sprite = null
export(int, 0, 1000) var recoil := 25
export(float, 0.01, 0.2) var recoil_duration := 0.1
export(int) var additional_cooldown_every_x_shots = -1
export(float) var additional_cooldown_multiplier = -1.0
export(int) var speed_percent_modifier := 0

var burning_data: BurningData = BurningData.new()
var attack_speed_mod := 0.0 # modified in items for class bonus

var col_b = "[/color]"

func get_text(base_stats: Resource, player_index: int, effects: Array = []) -> String:
	var text = ""

	text += Text.text("DAMAGE_FORMATTED", [get_col_a() + tr("STAT_DAMAGE") + col_b, get_damage_text(base_stats, player_index, effects)])
	text += "\n" + Text.text("CRITICAL_FORMATTED", [get_col_a() + tr("CRITICAL") + col_b, get_crit_damage_text(base_stats), get_crit_chance_text(base_stats)])
	text += "\n" + Text.text("STAT_FORMATTED", [get_col_a() + tr("COOLDOWN") + col_b, get_cooldown_text(base_stats, player_index)])

	text += get_knockback_text(base_stats)
	text += get_range_text(base_stats)
	text += get_bouncing_text(base_stats)
	text += get_piercing_text(base_stats)
	text += get_lifesteal_text(base_stats)
	text += get_additional_cooldown_text(base_stats, player_index)
	text += get_deal_dmg_on_return_text()
	text += get_alternate_attack_type_text()

	return text


func get_crit_damage_text(base_stats: Resource) -> String:
	var a = get_signed_col_a(crit_damage, base_stats.crit_damage)
	return a + "x" + str(crit_damage) + col_b


func get_crit_chance_text(base_stats: Resource) -> String:
	var a = get_signed_col_a(crit_chance, base_stats.crit_chance)
	return a + str(max(crit_chance * 100.0, 0)) + col_b


func get_cooldown_text(base_stats: Resource, player_index: int, multiplier: float = 1.0) -> String:
	var cd = get_cooldown_value(player_index, multiplier)
	var base_cd = get_base_cooldown_value(base_stats, multiplier)
	var a = get_signed_col_a(-cd, -base_cd)

	var step = 0.001 if cd < 0.2 else 0.01
	return a + str(stepify(cd, step)) + "s" + col_b


func get_additional_cooldown_text(base_stats: Resource, player_index: int) -> String:
	if additional_cooldown_every_x_shots == -1:
		return ""
	else:
		return "\n" + Text.text("ADDITIONAL_COOLDOWN_EVERY_X_SHOTS", [get_cooldown_text(base_stats, player_index, additional_cooldown_multiplier), str(additional_cooldown_every_x_shots)])


func get_cooldown_value(_player_index: int, _multiplier: float) -> float:
	return 0.0


func get_base_cooldown_value(_base_stats: Resource, _multiplier: float) -> float:
	return 0.0


func get_knockback_text(base_stats: Resource) -> String:
	if knockback == 0: return ""

	var a = get_signed_col_a(knockback, base_stats.knockback)

	if base_stats.can_have_negative_knockback and not base_stats.can_have_positive_knockback:
		a = get_signed_col_a(base_stats.knockback, knockback)

	return "\n" + Text.text("STAT_FORMATTED", [get_col_a() + tr("KNOCKBACK") + col_b, a + str(knockback)]) + col_b


func get_range_text(base_stats: Resource) -> String:
	var type_text = get_type_text()
	var a = get_signed_col_a(max_range, base_stats.max_range)

	var range_text = str(max_range) if min_range == 0 else str(min_range) + "-" + str(max_range)
	var base_range_text = str(base_stats.max_range) if base_stats.min_range == 0 else str(base_stats.min_range) + "-" + str(base_stats.max_range)

	var text = a + range_text + col_b

	if max_range != base_stats.max_range:
		text += get_init_a() + base_range_text + col_b

	return "\n" + Text.text("RANGE_FORMATTED", [get_col_a() + tr("STAT_RANGE") + col_b, text, type_text])


func get_damage_text(base_stats: Resource, player_index: int, effects: Array) -> String:
	return get_dmg_text_with_scaling_stats(base_stats, scaling_stats, 1, player_index, effects)


func get_dmg_text_with_scaling_stats(base_stats: Resource, p_scaling_stats: Array, nb_projectiles: int, player_index: int, effects: Array) -> String:

	var displayed_damage = damage

	for effect in effects:
		if effect is PlayerHealthStatEffect and effect.key == "stat_damage":
			displayed_damage += effect.get_bonus_damage(player_index)

	var a = get_signed_col_a(displayed_damage, base_stats.damage)
	var dmg_text = a + str(displayed_damage) + col_b

	var text = dmg_text if nb_projectiles == 1 else dmg_text + "x" + str(nb_projectiles)

	if displayed_damage != base_stats.damage:
		var initial_dmg_text = str(base_stats.damage) if nb_projectiles == 1 else str(base_stats.damage) + "x" + str(nb_projectiles)
		text += get_init_a() + initial_dmg_text + col_b

	text += " (" + WeaponService.get_scaling_stats_icon_text(p_scaling_stats) + ")"

	return text


func get_bouncing_text(_base_stats: Resource) -> String:
	return ""


func get_piercing_text(_base_stats: Resource) -> String:
	return ""


func get_type_text() -> String:
	return ""


func get_alternate_attack_type_text() -> String:
	return ""


func get_deal_dmg_on_return_text() -> String:
	return ""


func get_lifesteal_text(base_stats: Resource) -> String:
	if lifesteal <= 0.0: return ""
	var a = get_signed_col_a(lifesteal, base_stats.lifesteal)
	return "\n" + Text.text("STAT_FORMATTED", [get_col_a() + tr("LIFESTEAL") + col_b, a + str(round(lifesteal * 100.0))]) + "%" + col_b


func get_accuracy_text(base_stats: Resource) -> String:
	var a = get_signed_col_a(accuracy, base_stats.accuracy)
	var text = a + str(clamp(accuracy * 100.0, 0, 100)) + "%" + col_b

	if accuracy != base_stats.accuracy:
		text += get_init_a() + str(base_stats.accuracy * 100.0) + "%" + col_b

	return text


func get_accuracy() -> float:
	return accuracy


func get_signed_col_a(value: float, base_value: float) -> String:
	var col_pos_a = "[color="+ Utils.POS_COLOR_STR +"]"
	var col_neutral_a = "[color=white]"
	var col_neg_a = "[color="+ Utils.NEG_COLOR_STR +"]"
	if value > base_value: return col_pos_a
	elif value == base_value: return col_neutral_a
	else: return col_neg_a


func get_col_a() -> String:
	return "[color=#" +Utils.SECONDARY_FONT_COLOR.to_html()+"]"


func get_init_a() -> String:
	return " [color=" + Utils.GRAY_COLOR_STR + "]| "


func serialize() -> Dictionary:

	var shooting_sounds_paths = []

	for shooting_sound in shooting_sounds:
		shooting_sounds_paths.push_back(shooting_sound.resource_path)

	var serialized = {
		"type": "base",
		"cooldown": cooldown,
		"damage": damage,
		"accuracy": accuracy,
		"crit_chance": crit_chance,
		"crit_damage": crit_damage,
		"min_range": min_range,
		"max_range": max_range,
		"knockback": knockback,
		"knockback_piercing": knockback_piercing,
		"can_have_positive_knockback": can_have_positive_knockback,
		"can_have_negative_knockback": can_have_negative_knockback,
		"effect_scale": effect_scale,
		"scaling_stats": scaling_stats,
		"lifesteal": lifesteal,
		"shooting_sounds": shooting_sounds_paths,
		"sound_db_mod": sound_db_mod,
		"is_healing": is_healing,
		"recoil": recoil,
		"recoil_duration": recoil_duration,
		"additional_cooldown_every_x_shots": additional_cooldown_every_x_shots,
		"additional_cooldown_multiplier": additional_cooldown_multiplier
	}

	if custom_on_cooldown_sprite != null:
		serialized.custom_on_cooldown_sprite = custom_on_cooldown_sprite.resource_path

	return serialized


func deserialize_and_merge(serialized: Dictionary):

	shooting_sounds = []

	for sound_path in serialized.shooting_sounds:
		var sound = load(sound_path)
		if sound != null:
			shooting_sounds.push_back(sound)

	cooldown = serialized.cooldown as int
	damage = serialized.damage as int
	accuracy = serialized.accuracy
	crit_chance = serialized.crit_chance
	crit_damage = serialized.crit_damage
	min_range = serialized.min_range as int
	max_range = serialized.max_range as int
	knockback = serialized.knockback as int
	knockback_piercing = serialized.knockback_piercing if "knockback_piercing" in serialized else 0.0
	can_have_positive_knockback = serialized.can_have_positive_knockback if "can_have_positive_knockback" in serialized else true
	can_have_negative_knockback = serialized.can_have_negative_knockback if "can_have_negative_knockback" in serialized else false
	effect_scale = serialized.effect_scale
	scaling_stats = serialized.scaling_stats
	lifesteal = serialized.lifesteal
	sound_db_mod = serialized.sound_db_mod as int
	is_healing = serialized.is_healing
	recoil = serialized.recoil as int
	recoil_duration = serialized.recoil_duration
	additional_cooldown_every_x_shots = serialized.additional_cooldown_every_x_shots as int
	additional_cooldown_multiplier = serialized.additional_cooldown_multiplier

	if serialized.has("custom_on_cooldown_sprite"):
		custom_on_cooldown_sprite = load(serialized.custom_on_cooldown_sprite)
