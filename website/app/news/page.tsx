import NewsItem from "../ui/news-item"

export default function Page()
{
    // Yea, the way we are doing news right now is suuuuuper janky, but I don't have a whole lot a time to implement an
    //   honest-to-god CMS. So... cry about it I guess?

    let endOfSemesterContent = (
        <>
            <div className="space-y-2">
                <p>
                    The BU Weather Station team is working on getting the weather station in its final deployment state by
                    the end of the spring 2025 semester. This will include improvments to pretty much all parts of the
                    project, both hardware and software.
                </p>
                <p>
                    Users of the API can expect to see a new "v2" endpoint in the future, that will become the new
                    recommended way to access our data. Ideally, this endpoint will have some more features for data 
                    fetching, as well as higher reliability and the ability to return status via HTTP codes in 
                    the case the service is down. The development of this feature will likely be slower than all the
                    other improvements, for the sake of doing it right the first time.
                </p>
                <p>
                    The general user can expect to see further improvements to the UI, particularly improvments in the
                    representation of timed data, with proper spacing between data samples in graphs, which will help
                    more accurately portray weather events.
                </p>
                <p>
                    Furthermore, hardware overhauls will make the station more accurate and reliable, with a new
                    temperature sensor and solar panels for power. Ideally, we will also move the station to a spot
                    that can actually return accurate data that is not influenced by the BECC's glass and concrete.
                </p>
                <p>
                    Unfortunately, all of this development means that our services will be... spotty at best. We appreciate 
                    your patience while we finish out this project.
                </p>
                <p>
                    - Jacob S, Lead Student Developer - 2025-03-26
                </p>
            </div>
        </>
    );

    let summer_maintenance = (
        <>
            <div className="space-y-2">
                <p>
                    The BU Weather Station team has been hard at work making some
                    end-of-semester improvements to the station, and have
                    gotten the station to a point where we are happy with the data
                    it is currently exporting. A bad sensor was finally replaced on
                    2025-06-17, so the station is now back to reporting accurate data.
                </p>
                <p>
                    Currently, this project is entering maintenance mode in order
                    to make way for some major hardware overhauls that will allow
                    us to make all of the changes discussed in the post below.
                </p>
                <p>
                    Our services will remain available, and this station maintained
                    until a trasition is made to new hardware and software. The 
                    current API will become "legacy" but will still be compatible
                    with the new version of this station when it comes out.
                </p>
                <p>
                    If you have any questsions, comments, or concearns, reach
                    us on our <b>Contact</b> page above.
                </p>
                <p>
                    - Jacob S, Lead Student Developer - 2025-06-18
                </p>
            </div>
        </>
    );

    return (
        <>
            <div className="flex flex-col py-10 xl:mx-auto xl:w-1/2">
                <h1 className="text-6xl text-slate-700 font-bold">BU Weather Station News</h1>
                <p className="text-xl">
                    News about the BU Weather Station and it's attached services, straight from the development team.
                </p>
                <hr className="mt-5" />

                <div className="space-y-5">
                    <NewsItem 
                    title="Summer 25' and Future" 
                    content={summer_maintenance} />
                    <NewsItem 
                    title="End-Of-Semester Improvements" 
                    content={endOfSemesterContent} />
                </div>
            </div>
        </>
    )
}