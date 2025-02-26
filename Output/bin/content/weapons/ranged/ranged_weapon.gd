class_name RangedWeapon
extends Weapon


func _ready() -> void :
	var _projectile_shot = _shooting_behavior.connect("projectile_shot", self, "on_projectile_shot")


func on_projectile_shot(projectile: Node2D) -> void :
	if (effects.size() > 0 or RunData.get_player_effect("gain_stat_when_attack_killed_enemies", player_index).size() > 0) and is_instance_valid(projectile):
		var _killed_sthing = projectile._hitbox.connect("killed_something", self, "on_killed_something", [projectile._hitbox])

	var _hit_sthing = projectile.connect("hit_something", self, "on_weapon_hit_something", [projectile._hitbox])
