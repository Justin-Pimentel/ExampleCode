using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LycheeProjectile : MonoBehaviour {
    //How fast it goes
    public float speed;
    //How much wallop it packs
    public int damage;
    //How long it lasts
    public float lifetime;
    public float timer;

	// Use this for initialization
	void Start () {
        Invoke("destroyProjectile", lifetime);
        timer = 0;
	}

    // Update is called once per frame
    void Update()
    {
        transform.Translate(Vector2.up * speed * Time.deltaTime);
        timer += Time.deltaTime;
    }

    void OnTriggerEnter2D(Collider2D hit)
    {
        if(hit.gameObject.tag == "Player")
        {
            GameObject Player = hit.gameObject;
            Player.GetComponent<PlayerHealth>().TakeDamage(damage);
            destroyProjectile();
        }else if (hit.gameObject.tag == "Wall" && timer > 0.5f)
        {
            destroyProjectile();
            timer = 0;
        }
    }

    //Helper function to destroy projectile. Animations or sound fx can go here too for more feedback.
    void destroyProjectile()
    {
        Destroy(gameObject);
    }
}
