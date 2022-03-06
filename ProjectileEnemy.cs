using Godot;
using System;

public class ProjectileEnemy : BaseEnemy
{
	PackedScene MonsterBulletScene;
	protected AudioStreamPlayer2D combatAudioPlayer;
	public override void _Ready()
	{
		combatAudioPlayer = GetNode<AudioStreamPlayer2D>("AudioStreamPlayer2D");
		combatAudioPlayer.Stream = GD.Load<AudioStream>("res://Audio/Slime_Projectile.wav");
		base._Ready();
		MonsterBulletScene = GD.Load<PackedScene>("res://Scenes/MonsterBullet.tscn");
		
	}


	public override void _PhysicsProcess(float delta)
	{
		base._PhysicsProcess(delta);
		if (AnimSprite.Animation == "default")
		{
			if (AttackReady && PlayerFound)
			{
				//Fires bullet at player while they are within range and attack is off of cooldown
				AttackPlayer();
				this.LookAt(Player.Position);
			}
			else if ((Player.Position-this.Position).Abs().Length() > 250.0f && PlayerFound)
			{
				GD.Print((Player.Position - this.Position).Abs().Length());
				LinearVelocity = Player.Position - this.Position;
				Position += LinearVelocity.Clamped(1.25f) * delta * MovementSpeed;
				this.LookAt(Player.Position);

			}
		}
	}
	protected override void _on_EnemyBody_body_entered(Node body)
	{
		
	}

	protected override void _on_EnemyBody_body_exited(Node body)
	{

	}

	protected override void AttackPlayer()
	{
		MonsterBullet monsterbullet = (MonsterBullet)MonsterBulletScene.Instance();
		monsterbullet.Position = this.Position;
		monsterbullet.Rotation = this.Rotation;
		GetParent().AddChild(monsterbullet);
		AttackCooldown.Start(TimeBetweenAttacks);
		combatAudioPlayer.Play();
		AttackReady = false;
		//EmitSignal(nameof(PlayerDamaged), AttackPower);
	}

	protected override void _on_Timer_timeout()
	{
		AttackReady = true;
		GD.Print("Timer Elapsed");
	}

	protected override void _on_DetectionArea_body_entered(object body)
	{
		base._on_DetectionArea_body_entered(body);
		
	}

	protected override void _on_DetectionArea_body_exited(object body)
	{
		base._on_DetectionArea_body_exited(body);
		
	}
}
