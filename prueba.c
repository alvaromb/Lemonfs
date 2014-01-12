#include "bloques.h"
#include "ficheros_basico.h"
#include "ficheros.h"
#include "directorios.h"

#include <stdlib.h>

int main(int argc, char **argv) {
	
	if (argc == 3) {
		
		bmount(argv[1]);
		
		int nbloques = atoi(argv[2]);
		if (nbloques < 0) {
			nbloques = nbloques*(-1);
		}
		
		unsigned char buffer[TB];
		int i;
		for (i = 0; i<nbloques; i++) {
			bwrite(i, buffer);
		}
		
		initSB(nbloques, argv[1]);
		initMB(nbloques);
		initAI(nbloques);
		
		leerSB();
		
		mi_creat("/Documentos/");
		mi_creat("/Documentos/monologos/");
		mi_creat("/Documentos/monologos/BillHicks/");
		leerSB();
		sleep(10);
		char nombre[60];
		memset(nombre, '\0', 60);
		
		/* PRUEBAS ADICIONALES */
		for (i = 0; i<700; i++) {
			sprintf(nombre,"/Documentos/monologos/BillHicks/fichero-%d.dat",i);
			mi_creat(nombre);
		}
		
		char bfr[60000];
		memset(bfr, '\0', 60000);
		mi_dir("/Documentos/monologos/BillHicks/", bfr);
		printf("%s", bfr);
		mi_dir("/Documentos/monologos/", bfr);
		printf("%s", bfr);
		mi_dir("/Documentos/", bfr);
		printf("%s", bfr);
		mi_dir("/", bfr);
		printf("%s", bfr);
		leerSB();
		sleep(10);
		
		char text[60000];
		memset(text, '\0', 60000);
		strcpy(text, "[edit] Comedy Routine[edit] Salvation     That's why my girlfriend and I broke up: she wanted kids, and I … well, she wanted kids. [laughs] I had no idea her philosophy was that flawed. She goes, Wouldn't it be nice to have a kid? To have this fresh, clean slate which we could fill. A little clean spirit, innocent, and to fill it with good ideas. Yeah, yeah, how about this? If you're so fucking altruistic, why dont you \nleave the little clean spirit wherever it is right now? Okay? Horrible act, childbirth. Nightmare. Bringing … I would never bring a kid to this fucking planet.[edit] Arizona Bay    * You gotta see English crime. It's hilarious, you don't know if you're reading the front page or the comic section over there. I swear to God. I read an article front page of the paper one day, in England, 'Yes\nterday, some Hooligans knocked over a dustbin in Shaftesbury.' ...Wooooo. 'The hooligans are loose! The hooligans are loose! ...What if they become ruffians? I would hate to be a dustbin in Shaftesbury tonight. (to the tune of Behind Blue Eyes by The Who) No one knows what it's like... to be a dustbin... in Shaftesbury... with hooligans...'    * I got to see, while I was over there [England]\n footage of the Rodney King trial, which I had never seen while I was here. I saw footage over there of the Rodney King trial. I think I figured out why the L.A. riots occurred. Did you guys see these cops testifying, man? Did these guys have balls or what, man? These guys carry their balls in a wheelbarrow, man. 'Scuse me, 'scuse me. Man with big balls is here to testify." "Place your\n right testicle on the bible. Whishh BOOOM. This guy, Officer Coon … is life too fucking weird or what? Officer Coon looks in the camera and actually says: Oh, that Rodney King beating tape, it's all in how you look at it. The courtroom murmurs: Jesus, what balls. I've never seen bal\nls of this magnitude. He must have a specially fitted uniform in which to place these large testicles. That's incredible. All in how you look at it, Officer Coon?That's right. It's how you look at the tape.Well would you care to tell the court how you're looking at that?Yeah, okay, su\nre. It's how you look at it. For instance, well, if you play it backwards, you see us help King up and send him on his way.Mmm . . . not guilty." "'Scuse me, 'scuse me. Man with big balls has just been acquitted.    * You ever notice that everyone who believes in creationism looks really unevolved? \nEyes real close together, big furry hands and feet. I believe God created me in one day. Yeah, it looks like he rushed it.[edit] Revelations (1990's comedy routine)    * One of my big fears in life is that I'm gonna die, you know, and my parents are gonna come to clean out my apartment, find that porno wing I've been adding onto.    * How dare you have a wino tell me not to use drug\ns?         o On anti-drug television advertising    * You know we armed Iraq. I wondered about that too, you know. During the Persian Gulf war, those intelligence reports would come out: Iraq: incredible weapons – incredible weapons.How do you know that?Uh, well … we looked at the receipts.   * I'm so sick of arming the world and then sending troops over to destroy the fucking arms, you know what I mean? We keep arming these little countries, then we go and blow the shit out of them. We're like the bullies of the world, you know. We're like Jack Palance in \nthe movie Shane, throwing the pistol at the sheep herder's feet: Pick it up.I don't wanna pick it up mister, you'll shoot me.Pick up the gun.Mister, I don't want no trouble, huh. I just came down town here to get some hard rock candy for my kids, some gingham for my wife. I don't even know what gingham is, but she goes through about 10 rolls a week of that stuff. I ain't looking for no trouble, mister.Pick up the gun. Boom, boom. You all saw him. He had a gun.    * I love the movies, love 'em. Now, I'm watching Terminator 2 the other day, and I'm thinking to myself: They cannot top the s\ntunts in this film, they cannot top this shit, unless … they start using terminally ill people as stunt people in feature films … well, hear me out, 'cause I know to a lot of you this may seem a little cruel. Aww, Bill, terminally ill stunt people? That's cruel! You know what I think's cruel? Leaving your loved ones to die in a hospital room surrounded by strangers. Fuck that! Put 'em in the movies! What, you wanna let your grandmother live out her last days in a sterile hospital room, with translucent skin so thin you can see her last heartbeat work its way down her\n blue veins? Or do you want her to meet... Chuck Norris?  *      Fundamentalist Christianity. Fascinating. These people actually believe the world is 12 thousand years old. Swear to God! Based on what? I asked them. Well, we looked at all the people in the Bible, and we added them up all the way back to Adam and Eve, their ages – 12 thousand years. Well, how fucking scientific! Okay. I didn't know that you'd gone to so much trouble there. That's good.      You believe the world's 12 thousand years old? That's right. Okay, I got one word to ask you. A one word question. Ready? Uh-uh. Dinosaurs.      You know, the world's 12 thousand years old and dinosaurs existed, and\n they existed in that time … you'd think it would have been mentioned in the fucking Bible at some point. And lo, Jesus and the disciples walked to Nazareth. But the trail was blocked by a giant brontosaurus … with a splinter in his paw. And O, the disciples did run a-shrieking: 'What a big fucking lizard, Lord!' But Jesus was unafraid, and he took the splinter from the brontosaurus's paw, and the big lizard became his friend. And Jesus sent him to Scotland where he lived in a loch for O, so many years, inviting thousands of American tourists to bring their fat fucking families and \ntheir fat dollar bills. And O, Scotland did praise the Lord: 'Thank you, Lord. Thank you, Lord. Thank you, Lord.'    * God put [dinosaur fossils] here to test our faith!... I think God put you here to test my faith, dude.      :[On dinosaur fossils and their relationship to creationism]    * Seriously, does it bother anyone else that God might be fuckin' with our heads?! [Mimes digging] Ho ho ho! We'll see who believes in me now!    * You ever notice how people who believe in creationism look really unevolved?    * A lot of Christians wear crosses around their necks. You think when Jesus comes back, he ever wants to see a fucking cross? Kind of like going up to Jackie Onassis with a rifle pendant on, you know.    * I was just down in Dallas, Texas. You know, you\n can go down there and to Dealey Plaza where Kennedy was assassinated. And you can actually go to the sixth floor of the Schoolbook Depository. It's a museum called … The Assassination Museum. I think they named it that after the assassination. I can't be too sure of the chronology here, but … anyway, they have the window set up to look exactly like it did on that day. And it's really accurate, you know, 'cause Oswald's not in it.    * The war on drugs to me is absolutely phoney; it's so obviously phoney, okay? It's a war against our civil rights, that's all it is. They're using it to make us afraid to go out at night, afraid of each other, so that we lock ourselves in our homes and they get to suspend our rights one by one.    * Why is pot against the law? It wouldn't be because anyone can grow it, and therefore you can't make a profit off it, would it?    * I believe that God left certain drugs growing naturally upon our planet to help speed up and facilitate our evolution. Okay, not the most popular idea ever expressed. Either that or you're all real high and agreeing with me in the only way you can right now. [starts blinking]    * They lie ab\nout marijuana. Tell you pot-smoking makes you unmotivated. Lie! When you're high, you can do everything you normally do just as well … you just realize that it's not worth the fucking effort. There is a difference.    * Not all drugs are good. Some … are great.    * It is hard to quit smoking. Every one of them looks pretty good to me right now. Every cigarette looks like it was made by Go\nd, rolled by Jesus and lightly moistened shut with Claudia Schiffer's pussy lips right now.    * But you know, I saw this movie last year called Basic … Instinct. Okay. Now, Bill's quick capsule review: Piece of shit. Okay, now … yeah, yeah. End of story, by the way. Don't get caught up in that fevered hype phoney fucking debate about that piece of shit movie. Is it too sexist? And what a\nbout the movies, are they becoming too … You're just confused, you've forgotten how to judge correctly. Take a deep breath … look at it again. Oh, it's a piece of shit! Exactly, that's all it is. Satan squatted, let out a loaf, they put a fucking title on it, put it on a marquee – Satan's shit, piece of shit, walk away. But is it too … and what about the lesbian content, they … You're getting really baffled here. Piece of shit! Now walk away. That's all it is, it's nothing more! Free yourself, folks. If you see it: Piece of shit! Say it and walk away. You're right! You're right, not those fuckers who want to tell you how to think! You're fucking right!   *      And then I come to find out after that film, that all the lesbian sex scenes … let me repeat that. All the lesbian sex \nscenes … were cut out of that film because the test audience was turned off by them.      Boy, is my thumb not on the pulse of America.      I don't want to seem like Randy Pan, the Goat Boy, but that was the only reason I went to that piece of shit. If I had been in that test audience, the only one out front protesting that film would have been Michael Douglas, demanding his part be put back in, all right. I swear I was in that movie. I swear I was!Gee, Mike. The movie started. Sharon Stone was eating another woman for an hour and a half. Then the credits rolled. I don't remember seeing your scrawny ass, Mike.Was Bill Hicks in that test audience?  *      By the way, if anyone here is in advertising or marketing, kill yourself.      Just a little thought. I'm ju\nst trying to plant seeds. Maybe one day, they'll take root. I don't know. You try. You do what you can. Kill yourself.      Seriously, though. If you are, do. No, really. There's no rationalisation for what you do, and you are Satan's little helpers, okay? Kill yourself. Seriously. You are the ruiner of all things good, seriously. No, this is not a joke, if you're going: There's going to be a joke coming. There's no fucking joke coming. You are Satan's spawn, filling the world with bile and garbage. You are fucked, and you are fucking us. Kill yourself, it's the only way to save your fucking soul. Kill yourself. Planting seeds      I know all the marketing people are going: He's doing a joke. There's no joke here whatsoever. Suck a tail-pipe, fucking hang yourself, borrow a gun \nfrom a Yank friend – I don't care how you do it. Rid the world of your evil fucking machinations.      I know what all the marketing people are thinking right now too. Oh, you know what Bill's doing? He's going for that anti-marketing dollar. That's a good market, he's very smart. Oh man. I am not doing that, you fucking evil scumbags!Oh, you know what Bill's doing now? He's going for th\ne righteous indignation dollar. That's a big dollar. Lot of people are feeling that indignation, we've done research. Huge market. He's doing a good thing. God damn it, I'm not doing that, you scumbags. Quit putting a goddamn dollar sign on every fucking thing on this planet!Oh, the anger dollar. Huge. Huge in times of recession. Giant market, Bill's very bright to do that. God, I'm just caught in a fucking web.Oh, the trapped dollar. Big dollar, huge dollar. Good market, look at our research. We see that many people feel trapped. If we play to that and then separate them into the trapped dollar … How do you live like that? And I bet you sleep like fucking babies at night, don't you?hat did you do today, honey?Oh, we made arsenic childhood food. Now, good night. \nYeah, we just said, you know, is your baby really too loud? You know … yeah, the mums will love it, yeah. Sleep like fucking children, don't you? This is your world, isn't it?    * Go back to bed, America. Your government has figured out how it all transpired. Go back to bed, America. Your government is in control again. Here. Here's American Gladiators. Watch this, shut up. Go back to bed, America. Here is American Gladiators. Here is 56 channels of it! Watch these pituitary retards bang their fucking skulls together and congratulate you on living in the land of freedom. Here you go, America! You are free to do what we tell you! You are free to do what we tell you!    * I am available for children's parties, by the way … I'm Beelzebozo, The Clown from hell. [singi\nng] It's Beelzebozo time …    * The world is like a ride at an amusement park. And when you choose to go on it, you think it's real because that's how powerful our minds are. And the ride goes up and down and round and round. It has thrills and chills and it's very brightly coloured and it's very loud and it's fun, for a while. Some people have been on the ride for a long time, and they begin to question: is this real, or is this just a ride? And other people have remembered, and they come back to us. They say, Hey, don't worry, don't be afraid ever, because – this is just a ride. And we … kill those people. Shut him up. We have a lot invested in this ride. Shut him up. Look at my furrows of worry. Look at my big bank account and my family. This just has to be r\neal. It's just a ride. But we always kill those good guys who try and tell us that, you ever notice that? And let the demons run amok. Jesus, murdered; Martin Luther King, murdered; Malcolm X, murdered; Gandhi, murdered; John Lennon, murdered; Reagan … wounded. But it doesn't matter, because – it's just a ride. And we can change it any time we want. It's only a choice. No effort, no work, no job, no savings and money. A choice, right now, between fear and love. The eyes of fear want you to put bigger locks on your doors, buy guns, close yourself off. The eyes of love instead see all of us as one. Here's what we can do to change the world, right now, to a better ride. Take all that money we spend on weapons and defenses each year and instead spend it feeding and clothi\nng and educating the poor of the world, which it would pay for many times over, not one human being excluded, and we could explore space, together, both inner and outer, forever, in peace.    * In many parts of our troubled world, people are yelling 'Revolution!'. In Tennessee they're yelling 'Evolution! We want our thumbs!'  *      But I'll tell you this. Where's this idea that childbirth is a miracle come from? I missed that fucking meeting, okay. It's a miracle. Childbirth is a miracle. No, it's not. No more a miracle than eating food and a turd coming out of your ass, all right. It's a chemical reaction, that's all it fucking is. You wanna know what a miracle is? Raising a kid that doesn't talk in a movie theatre. Okay, there. There is a goddamn miracle.      Not a miracle if every nine months any yin yang in the world can drop a litter of these mewling cabbages on our planet. And just in case you haven't seen the single mom statistics lately, the miracle is spreading like wild-fire. Hallelujah! Trailer parks and council flats all over the world, just filling up with little miracles. THUNK, THUNK, THUNK – like frogs laying eggs – THUNK. Look at all my little miracles – THUNK – filling up my trailer like a sardine can. THUNK. You know what \nwould be a real miracle? If I could remember your daddy's name. THUNK. I guess I'll have to call you Lorry Driver Junior. THUNK. That's all I remember about your daddy, was his fuzzy little pot-belly riding on top of me shooting his caffeine-ridden semen into my belly to produce my little water-headed miracle baby. Oh, there's your brother, Pizza Boy Delivery Junior.       Just hold on for a minute. Let's figure out this food/air deal, okay? Okay. I'm just weird, you know. How about have a neat world for kids to come to? Okay, it's me. Fuck it.[edit] Relentless    * People come up to me and say, What's wrong?Nothing.Well, it takes more energy to frown than it does to smile.\nYeah, you know it takes more energy to point that out than it does to leave me alone?    * I smoke. If this bothers anyone, I recommend you look around the world in which we live, and … I don't know, shut your fucking mouth?   *      Non-smokers die every day. Sleep tight!    * See, I know you entertain some kind of eternal life fantasy because you've chosen not to smoke; let me be the first to pop that fucking bubble and send you hurtling back to reality – because you're dead too. And you know what doctors say: Shit, if only you'd smoked, we'd have the technology to help you. It's you people dying from nothing who are screwed.    * And I'll tell you something too. That's starting to annoy me about UFOs, the fact that they cross galaxies or universes to visit us\n, and always end up in places like … Fyffe fucking Alabama. Maybe these aren't super-intelligent beings, you know what I mean?    * To my delight, I find that there is a different warning on each pack of cigarettes. Mine says: Warning: Smoking can cause fetal damage or premature birth. Fuck it – I've found my brand! Yeah, give me a carton of Low Birth Weights Just don't get the ones that say lung cancer, you know? Shop around, find a disease you can live with. What are you smoking, Dave?Throat Polyps. Want one?No thanks, I'll stick with my Yellow Fingernails.    * It's really weird how your life changes. Tonight I'm drinking water. Four years ago? Opium. Night and day, you know?    * Your denial is beneath you, and thanks to the use of hallucinogenic drugs, I see through you.    * The rock stars today who don't do drugs and who in fact speak out against \ndrugs – We're rock against drugs! – boy, they suck.    * You see, I think drugs have done some good things for us. I really do. And if you don't believe drugs have done good things for us, do me a favor. Go home tonight. Take all your albums, all your tapes and all your CDs and burn them. 'Cause you know what, the musicians that made all that great music that's enhanced your lives throughout the years … rrreal fucking high on drugs. The Beatles were so fucking high they let Ringo sing a few tunes.    *      They say rock n' roll is the devil's music. Well, let's say that it is; I've got news for you. Let's say that rock n' roll is the devil's music and we know it for a fact to be the absolutely, unequivocally true.      Boy, at least he fucking jams! Ha ha ha ha! Okay?\n Did you hear that correctly? If it's a choice between eternal hell and good tunes and eternal heaven and new kids on the fucking block … I'm gonna be surfing on the lake of fire, rocking out … high five at Satan every time I pass the motherfucking shore.    * You know, if you play New Kids on the Block albums backwards … they sound better. Gives them that edge they're missing, puts some fucking hair on their balls.    * Come on, Bill, they're the New Kids. They're so good and so clean-cut and they're such a good image for the children. Fuck that! When did mediocrity and banality become a good image for your children? I want my children to listen to people who fucking rocked! I don't care if they died in puddles of their own vomit. I want someone who plays from his fuck\ning heart!    *      You know what causes sexual thoughts? I'm gonna clear the air for you tonight. I'm gonna end this debate, hopefully once and for all while on this planet, 'cause outer space awaits our presence, we are better and more unique creatures than this and all eternity is our playground, so let me go ahead and clear this one issue up once and for all and let's move on to real issues.      Here's what causes sexual thoughts: having a dick.    * What do atheists scream when they cum? Oh, chemical chance! chemical chance!    *      Are there actually women in the world who do not like to give blow jobs? See a lot of guys on dates got their fingers crossed here tonight … Answer him, honey, go ahead. Let's hear how you feel about this right now.      A woman one night\n yelled out, Yeah, you ever try it? I said, Yeah. Almost broke my back. It's that one vertebrae, I swear to God, it's that close. I think that vertebrae is going to be the thing to go in our next evolutionary step. Just a theory...and a fervent prayer! Yeah, now all the guys are going, Honey, I have no idea what he's talking about. I think he's a devil-child. That may be true, but guys … yyyou know what I'm talking about. I can speak for every guy in this room here tonight. Guys, if you could blow yourselves, ladies, you'd be in this room alone right now … watching an empty stage.      Boy, my folks are proud of me! Bill, honey, you still doing that suck your own cock bit?Yeah, mom.Good, baby, that's such a crowd-pleaser. How clever of you to come up with the su\nck your own cock bit, honey. You're so clever, it makes your mama's bosom swell with pride. Knowing her son is travelling the world, using his given surname, going up in front of rooms of total strangers and doing the suck your own cock piece!Thanks, mom.No biggie    * I actually did that act one night in the south. Then, after the show, these three rednecks came up to me. Hey, buddy! We're Christians and we didn't like what you said. I said, Then forgive me.Later on, when I was hanging from the tree …    * I don't do drugs anymore... than, I'd say, the average touring funk band.[edit] Rant in E-Minor    * They [Australians] celebrate Easter the exact same way we do: commemorating the death and resurrection of Jesus by telling our children a giant bunny ra\nbbit … left chocolate eggs in the night. Now, I wonder why we're fucked up as a race. Anybody got any idea? You know, I've read the Bible. I can't find the word bunny or chocolate anywhere in the fucking book. Where do they come up with this shit? Why not goldfish left Lincoln logs in your sock drawer? As long as you're making shit up, you know, go hog-wild. At least the goldfish with a Lincoln log on its back going across your carpet has some miraculous connotations. [in faux British accent] Mummy, I woke up today and there was a Lincoln log in me sock drawer.That's the story of Jesus!    * It's a half-hour weekly show that I will host, entitled Let's Hunt and Kill Billy Ray Cyrus. It's a fairly self-explanatory concept: Each week we let the hounds of hell loose and cha\nse that no-talent cracker asshole across the globe until I catch him by that fruity ponytail, pull him to his knees, and put a shotgun in his mouth like a big black cock of death [boom] and we'll be back in '95 with Let's Hunt and Kill Michael Bolton.   *      Do you all have different books in the Bible than I do? Are you all Gideons? Who are the fucking Gideons? Ever met one, no! Ever seen one, no! But they're all over the fucking world, putting Bibles in hotel rooms! Every one of them: This Bible was placed here by a Gideon. When? I've been here all day and I ain't seen shit! I saw the housekeeper come and go, I saw the minibar guy come and go, I've never laid eyes on a fucking Gideon! What are they, ninjas? Where are they? Where are they from? Gidea? Who the fuck are these people?!\n");
		
		mi_write("/Documentos/monologos/BillHicks/fichero-690.dat", text, 0, strlen(text));
		printf("Hemos escrito el texto, habrá 25 bloques libres menos\n");
		leerSB();
		sleep(10);
		
		
		struct STAT estado;
		mi_stat("/Documentos/monologos/BillHicks/fichero-690.dat", &estado);
		
		char bf[TB];
		
		for (i = 0; (i*TB) < estado.t_bytes; i++) {
			mi_read("/Documentos/monologos/BillHicks/fichero-690.dat", bf, (i*TB), TB);
			/* Imprimimos por pantalla */
			printf("%s", bf);
		}
		sleep(10);
		
		mi_unlink("/Documentos/monologos/BillHicks/fichero-690.dat");
		printf("Hemos eliminado el fichero 690, habrá más bloques libres y un i-nodo más\n");
		leerSB();	
		
		memset(bfr, '\0', 60000);
		mi_dir("/Documentos/monologos/BillHicks/", bfr);
		printf("%s", bfr);
		mi_dir("/Documentos/monologos/", bfr);
		printf("%s", bfr);
		mi_dir("/Documentos/", bfr);
		printf("%s", bfr);
		mi_dir("/", bfr);
		printf("%s", bfr);
		
		bumount();
	}
	
	return (0);
}